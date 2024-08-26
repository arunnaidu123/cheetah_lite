/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 The SKA organisation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "cheetah/psrdada/DadaReadClient.h"
#include "panda/Error.h"
#include "panda/Log.h"
#include "ipcbuf.h"
#include "ipcutil.h"
#include "tmutil.h"
#include <sys/ipc.h>
#include <streambuf>
#include <mutex>

#define IPCBUF_VIEWER  1  /* connected */
#define IPCBUF_READER  5  /* one process that reads from the buffer */
#define IPCBUF_READING 6  /* start-of-data flag has been raised */
#define IPCBUF_VIEWING 8  /* currently viewing */
#define IPCBUF_VSTOP   9  /* end-of-data while viewer */



extern "C"
{
    char* ipcbuf_get_next_read_work(ipcbuf_t* id, uint64_t* bytes, int flag);
}

namespace ska {
namespace cheetah {
namespace psrdada {

DadaReadClient::DadaReadClient(key_t key,panda::Engine& engine, NextSequenceCallback const& next_sequence_callback)
    : DadaClientBase(key, "read_client")
    , _next_sequence_callback(next_sequence_callback)
    , _current_block(nullptr)
    , _current_reader(nullptr)
    , _locked(false)
    , _engine(engine)
    , _destructor_flag(std::make_shared<bool>(false))
{
    lock();
    //_engine.post(std::bind(&DadaReadClient::do_next_sequence<NextSequenceCallback>
                          //, this, _destructor_flag, _next_sequence_callback));
}

void DadaReadClient::start()
{
    _engine.post(std::bind(&DadaReadClient::do_next_sequence<NextSequenceCallback>
                          , this, _destructor_flag, _next_sequence_callback));
}

DadaReadClient::~DadaReadClient()
{
    PANDA_LOG_DEBUG << "Destructing the read client";
    stop();
    unlock();
    while(_destructor_flag.use_count() != 1 )
    {
        _engine.poll_one();
    }
}


void DadaReadClient::stop()
{
    if (!*_destructor_flag)
    {
        *_destructor_flag = true;
        PANDA_LOG_DEBUG << "Stop!";
        release_data_block();
    }
}


void DadaReadClient::next_sequence()
{
    next_sequence(_next_sequence_callback);
}


std::unique_ptr<detail::RawBytesReader>& DadaReadClient::acquire_data_block()
{
    release_data_block();
    PANDA_LOG_DEBUG << id() << "Acquiring next data block";
    uint64_t block_idx, nbytes = 0;
    char* tmp;
    do {
        tmp = open_block_read(_hdu->data_block, &nbytes, &block_idx);
        if (!tmp)
        {
            if (eod())
            {
                _current_block.reset(nullptr);
                PANDA_LOG << id() << "Reached EOD in data ring";
                _current_reader.reset(nullptr);
                return _current_reader;
            }
            if(*_destructor_flag)
            {
                release_data_block();
                _current_reader.reset(nullptr);
                return _current_reader;
            }
        }
    } while(!tmp);

    if(*_destructor_flag)
    {
        release_data_block();
    }
    else
    {
        std::lock_guard<std::mutex> lock(_block_mutex);
        PANDA_LOG_DEBUG << id() << "Acquired block " << block_idx;
        _current_block.reset(new detail::RawBytes(tmp, data_buffer_size(), nbytes));
        PANDA_LOG_DEBUG << id() << "Data block used/total bytes = "
            << _current_block->used_bytes() <<"/"<<_current_block->total_bytes();
        _current_reader.reset(new detail::RawBytesReader(*_current_block));
    }
    return _current_reader;
}


void DadaReadClient::release_data_block()
{

    PANDA_LOG_DEBUG << id() << "Releasing data block";
    std::lock_guard<std::mutex> lock(_block_mutex);
    if (!_current_block)
    {
        if(_hdu->data_block)
        {
            ipcio_close_block_read (_hdu->data_block, 0);
        }
    }
    else if (ipcio_close_block_read (_hdu->data_block, _current_block->used_bytes()) < 0)
    {
        _log.write(LOG_ERR, "close_buffer: ipcio_close_block_read failed\n");
        throw panda::Error("Could not close ipcio data block");
    }
    _current_reader.reset(nullptr);
    _current_block.reset(nullptr);
    PANDA_LOG_DEBUG << "Release done";
}


void DadaReadClient::flush()
{
    PANDA_LOG_DEBUG << id() << "Reader flush called";
    while (!eod() && !*_destructor_flag)
    {
        if(!acquire_data_block())
        {
            break;
        }
        PANDA_LOG_DEBUG << "Return from acquire";
    }
    release_data_block();
    reset();
}


void DadaReadClient::lock()
{
    if (!_connected)
    {
        throw panda::Error("Lock requested on unconnected HDU");
    }
    if (dada_hdu_lock_read (_hdu) < 0)
    {
        _log.write(LOG_ERR, "open_hdu: could not lock read\n");
        throw panda::Error("Error locking HDU");
    }
    _locked = true;
}

void DadaReadClient::unlock()
{
    if (!_locked)
    {
        throw panda::Error("Release requested on unlocked HDU");
    }
    if (dada_hdu_unlock_read (_hdu) < 0)
    {
        _log.write(LOG_ERR, "open_hdu: could not release read\n");
        throw panda::Error("Error releasing HDU");
    }
    _locked = false;
}

void DadaReadClient::reset()
{
    if(!*_destructor_flag)
    {
        unlock();
        lock();
    }
}

bool DadaReadClient::eod() const
{
    return (bool) (ipcbuf_eod((ipcbuf_t *)(_hdu->data_block)));
}


// Overloading the open_block method from psrdada for multi-threading purposes

char* DadaReadClient::open_block_read (ipcio_t* ipc, std::uint64_t *curbufsz, std::uint64_t *block_id)
{
    if (ipc->bytes != 0)
    {
        PANDA_LOG_ERROR << "ipcio_open_block_read: ipc->bytes != 0";
        return 0;
    }

    if (ipc->curbuf)
    {
        PANDA_LOG_ERROR << "ipcio_open_block_read: ipc->curbuf != 0";
        return 0;
    }

    if (ipc -> rdwrt != 'r' && ipc -> rdwrt != 'R')
    {
        PANDA_LOG_ERROR << "ipcio_open_block_read: ipc -> rdwrt != [rR]";
        return 0;
    }
  // Test for eod

    if (ipcbuf_eod((ipcbuf_t*)ipc))
    {
        PANDA_LOG_ERROR << "ipcio_open_block_read: ipcbuf_eod true, returning null ptr";
        return 0;
    }
    ipc->curbuf = ipcbuf_get_next_read ((ipcbuf_t*)ipc, &(ipc->curbufsz));

    if (!ipc->curbuf)
    {
        //PANDA_LOG_ERROR << "ipcio_open_block_read: could not get next block rdwrt=%c" << ipc -> rdwrt;
        return 0;
    }

    *block_id = ipcbuf_get_read_index ((ipcbuf_t*)ipc);
    *curbufsz = ipc->curbufsz;
    ipc->bytes = 0;

    if(!*_destructor_flag)
    {
        return ipc->curbuf;
    }
    else
    {
        return nullptr;
    }
}

static
char* psrdada_ipcbuf_get_next_read_work (ipcbuf_t* id, uint64_t* bytes, int flag)
{
    int iread = -1;
    uint64_t bufnum;
    uint64_t start_byte = 0;
    ipcsync_t* sync = 0;

    if (ipcbuf_eod (id))
        return NULL;

    sync = id->sync;

    if (ipcbuf_is_reader (id))
    {
        iread = id->iread;

        /* decrement the buffers written semaphore */
        if (ipc_semop (id->semid_data[iread], IPCBUF_FULL, -1, flag) < 0)
        {
            return NULL;
        }

        if (id->state == IPCBUF_READER)
        {
            id->xfer = sync->r_xfers[iread] % IPCBUF_XFERS;

            id->state = IPCBUF_READING;
            id->sync->r_states[iread] = IPCBUF_READING;

            sync->r_bufs[iread] = sync->s_buf[id->xfer];
            start_byte = sync->s_byte[id->xfer];

      /* increment the start-of-data acknowlegement semaphore */
            if (ipc_semop (id->semid_data[iread], IPCBUF_SODACK, 1, flag) < 0)
            {
                PANDA_LOG_ERROR << "ipcbuf_get_next_read: error increment SODACK\n";
                return NULL;
            }

        }

        bufnum = sync->r_bufs[iread];

    }
    else
    {
        // TODO - check if we should always just use id->iread = 0 for this??
        iread = 0;

        if (id->state == IPCBUF_VIEWER)
        {

            id->xfer = sync->r_xfers[iread] % IPCBUF_XFERS;
            id->state = IPCBUF_VIEWING;

            id->viewbuf = sync->s_buf[id->xfer];
            start_byte = sync->s_byte[id->xfer];

            if (sync->w_buf_curr > id->viewbuf + 1)
            {
                id->viewbuf = sync->w_buf_curr - 1;
                start_byte = 0;
            }
        }

    /* Viewers wait until w_buf_curr is incremented without semaphore operations */
        while (sync->w_buf_curr <= id->viewbuf)
        {

            // AJ added: sync->r_bufs[iread] to ensure that a buffer has been read by a reader
            if (sync->eod[id->xfer] && sync->r_bufs[iread] && sync->r_bufs[iread] == sync->e_buf[id->xfer])
            {
                id->state = IPCBUF_VSTOP;
                break;
            }

            float_sleep (0.1);
        }

        if (id->viewbuf + sync->nbufs < sync->w_buf_curr)
            id->viewbuf = sync->w_buf_curr - sync->nbufs + 1;

        bufnum = id->viewbuf;
        id->viewbuf ++;
    }

    bufnum %= sync->nbufs;

    if (bytes)
    {
        if (sync->eod[id->xfer] && sync->r_bufs[iread] == sync->e_buf[id->xfer])
        {
            *bytes = sync->e_byte[id->xfer] - start_byte;
        }
        else
            *bytes = sync->bufsz - start_byte;
    }

    return id->buffer[bufnum] + start_byte;
}


char* DadaReadClient::ipcbuf_get_next_read (ipcbuf_t* id, uint64_t* bytes)
{
    return psrdada_ipcbuf_get_next_read_work (id, bytes, IPC_NOWAIT);
}

} // namespace psrdada
} // namespace cheetah
} // namespace ska
