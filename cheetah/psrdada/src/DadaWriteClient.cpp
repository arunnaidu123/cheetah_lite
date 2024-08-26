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

#include "cheetah/psrdada/DadaWriteClient.h"
#include "panda/Log.h"
#include "panda/Error.h"
#include <cstddef>

namespace ska {
namespace cheetah {
namespace psrdada {

DadaWriteClient::DadaWriteClient(key_t key, NewSequenceCallback const& new_sequence_callback)
    : DadaClientBase(key, "write_client")
    , _new_sequence_callback(new_sequence_callback)
    , _current_block(nullptr)
    , _current_writer(nullptr)
    , _locked(false)
{
    lock();
    new_sequence_impl();
}

DadaWriteClient::~DadaWriteClient()
{
    release_data_block();
    unlock();
}

void DadaWriteClient::new_sequence()
{
    reset();
    new_sequence_impl();
}

void DadaWriteClient::new_sequence_impl()
{
    PANDA_LOG_DEBUG << id() << "Acquiring next header block";
    char* tmp = ipcbuf_get_next_write(_hdu->header_block);
    if (!tmp)
    {
        _log.write(LOG_ERR, "Could not acquire next header block\n");
        throw panda::Error("Could not acquire next header block");
    }
    detail::membuf sbuf(tmp, tmp + header_buffer_size());
    auto deleter = [&](std::ostream* i)
    {
        delete i;
        std::size_t used_bytes = std::distance(tmp, sbuf.writer_position());
        PANDA_LOG_DEBUG << id() << "Releasing current header block";
        PANDA_LOG_DEBUG << id() << "Header bytes used " << used_bytes;
        if (ipcbuf_mark_filled(_hdu->header_block, used_bytes) < 0)
        {
            _log.write(LOG_ERR, "Could not mark filled header block\n");
            throw panda::Error("Could not mark filled header block");
        }
    };
    std::unique_ptr<std::ostream, decltype(deleter)> header(new std::ostream(&sbuf), deleter);
    _new_sequence_callback(*header);
    if (header->fail())
    {
        throw panda::Error("Failbit set on header stream after writing");
    }
}

std::unique_ptr<detail::RawBytesWriter>& DadaWriteClient::acquire_data_block()
{
    release_data_block();
    PANDA_LOG_DEBUG << id() << "Acquiring next data block";
    uint64_t block_idx;
    char* tmp = ipcio_open_block_write(_hdu->data_block, &block_idx);
    if (!tmp)
    {
         _log.write(LOG_ERR, "Could not get data block\n");
         throw panda::Error("Could not open block to write");
    }
    _current_block.reset(new detail::RawBytes(tmp, data_buffer_size()));
    PANDA_LOG_DEBUG << id() << "Acquired data block " << block_idx;
    _current_writer.reset(new detail::RawBytesWriter(*_current_block));
    return _current_writer;
}

void DadaWriteClient::release_data_block()
{
    if (!_current_block) return;
    PANDA_LOG_DEBUG << id() << "Releasing data block";
    PANDA_LOG_DEBUG << id() << _current_block->used_bytes() << " bytes written";
    if (ipcio_close_block_write (_hdu->data_block, _current_block->used_bytes()) < 0)
    {
        _log.write(LOG_ERR, "close_buffer: ipcio_close_block_write failed\n");
        throw panda::Error("Could not close ipcio data block");
    }
    _current_writer.reset(nullptr);
    _current_block.reset(nullptr);
}

void DadaWriteClient::lock()
{
    if (!_connected)
    {
        throw panda::Error("Lock requested on unconnected HDU");
    }
    PANDA_LOG_DEBUG << this->id() << "Acquiring writing lock on dada buffer";
    if (dada_hdu_lock_write (_hdu) < 0)
    {
        _log.write(LOG_ERR, "open_hdu: could not lock write\n");
        throw panda::Error("Error locking HDU");
    }
    _locked = true;
}

void DadaWriteClient::unlock()
{
    if (!_locked)
    {
        throw panda::Error("Release requested on unlocked HDU\n");
    }
    PANDA_LOG_DEBUG << this->id() << "Releasing writing lock on dada buffer";
    if (dada_hdu_unlock_write (_hdu) < 0)
    {
        _log.write(LOG_ERR, "open_hdu: could not release write\n");
        throw panda::Error("Error releasing HDU");
    }
    _locked = false;
}

void DadaWriteClient::reset()
{
    release_data_block();
    unlock();
    lock();
}

void DadaWriteClient::write_eod()
{
    release_data_block();
}

} // namespace psrdada
} // namespace cheetah
} // namespace ska
