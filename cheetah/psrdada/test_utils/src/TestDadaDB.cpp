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
#include "cheetah/psrdada/test_utils/TestDadaDB.h"
#include "panda/Error.h"
#include "panda/Log.h"
#include <sys/shm.h>
#include <time.h>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <unistd.h>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace test_utils {
namespace details {
    /**
     * @brief      Generate a random shared memory key for use with PSRDADA
     *
     * @details     PSRDADA requires two consequtive shared memory keys. We
     *             first generate an even numbered key of the form 0xdadaXXXX
     *             where XXXX is a random 16-bit number. This key and the key
     *             that follows it (i.e. key+1) will be used when allocating a
     *             new DADA buffer
     *
     * @return     A 32-bit share memory key with the top two bytes set to 0xdada
     */
    key_t random_dada_key()
    {
        int max_attempts = 20, attempts = 0;
        srand (time(NULL) + ::getpid());
        while (attempts < max_attempts)
        {
            key_t key = 0xdada0000 + 2*((rand() % (1<<16))/2);
            if ((shmget(key, 0, 0)==-1) && (shmget(key+1, 0, 0)==-1))
            {
                return key;
            }
            ++attempts;
        }
        throw std::runtime_error("Unable to find free dada key");
    }

} // namespace detail

TestDadaDB::TestDadaDB(uint64_t nbufs, uint64_t bufsz, uint64_t nhdrs,  uint64_t hdrsz)
    : _nbufs(nbufs)
    , _bufsz(bufsz)
    , _nhdrs(nhdrs)
    , _hdrsz(hdrsz)
    , _dada_key(0)
    , _data_block(IPCBUF_INIT)
    , _header(IPCBUF_INIT)
    , _data_blocks_created(false)
    , _header_blocks_created(false)
{
}

TestDadaDB::~TestDadaDB()
{
    destroy();
}

void TestDadaDB::create()
{
    std::lock_guard<std::mutex> lock(_lock);
    if (_data_blocks_created)
        throw std::runtime_error("DADA data blocks already created");

    _dada_key = detail::random_dada_key();
    PANDA_LOG << "Using DADA key: " << std::hex << _dada_key << std::dec << "\n";

    //Here we always assume that the number of readers will be 1.
    if (ipcbuf_create (&_data_block, _dada_key, _nbufs, _bufsz, 1) < 0) {
        do_destroy();
        throw std::runtime_error("Could not create DADA data block");
    }
    _data_blocks_created = true;
    PANDA_LOG_DEBUG << "Create DADA data block with nbufs=" << _nbufs << " bufsz=" << _bufsz << "\n";

    if (_header_blocks_created)
        throw std::runtime_error("DADA header blocks already created");
    if (ipcbuf_create (&_header, _dada_key + 1, _nhdrs, _hdrsz, 1) < 0) {
        do_destroy();
        throw std::runtime_error("Could not create DADA header block");
    }
    _header_blocks_created = true;
    PANDA_LOG_DEBUG << "Create DADA header block with nhdrs=" << _nhdrs << " hdrsz=" << _hdrsz << "\n";
}

void TestDadaDB::destroy()
{
    std::lock_guard<std::mutex> lock(_lock);
    PANDA_LOG_DEBUG << "Destroying the buffers";
    do_destroy();
}

void TestDadaDB::do_destroy()
{
    if (_data_blocks_created)
    {
        ipcbuf_connect(&_data_block, _dada_key);
        ipcbuf_destroy(&_data_block);
        _data_blocks_created = false;
    }
    if (_header_blocks_created)
    {
        ipcbuf_connect(&_header, _dada_key + 1);
        ipcbuf_destroy(&_header);
        _header_blocks_created = false;
    }
}

uint64_t TestDadaDB::num_data_buffers() const
{
    return _nbufs;
}

uint64_t TestDadaDB::data_buffer_size() const
{
    return _bufsz;
}

uint64_t TestDadaDB::num_header_buffers() const
{
    return _nhdrs;
}

uint64_t TestDadaDB::header_buffer_size() const
{
    return _hdrsz;
}

key_t TestDadaDB::key() const
{
    return _dada_key;
}

} // namespace test_utils
} // namespace psrdada
} // namespace cheetah
} // namespace ska
