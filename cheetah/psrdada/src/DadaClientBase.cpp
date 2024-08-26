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
#include "cheetah/psrdada/DadaClientBase.h"
#include "panda/Log.h"
#include "panda/Error.h"
#include <fstream>
#include <sstream>
#include <random>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

membuf::membuf(char* begin, char* end)
{
    this->setg(begin, begin, end);
    this->setp(begin, end);
}

char* membuf::reader_position() const
{
    return this->gptr();
}

char* membuf::writer_position() const
{
    return this->pptr();
}

std::string generate_logger_tag(std::string const& basename)
{
    auto gen = std::mt19937{std::random_device{}()};
    auto dist = std::uniform_int_distribution<int>{100000000, 999999999};
    std::stringstream name;
    name << basename << "_" << dist(gen);
    return name.str();
}

} // namespace detail

DadaClientBase::DadaClientBase(key_t key, std::string const& logger_name)
    : _key(key)
    , _log(detail::generate_logger_tag(logger_name))
{
    std::stringstream _key_string_stream;
    _key_string_stream.exceptions(std::ifstream::failbit);
    _key_string_stream << "["<< std::hex << _key << std::dec << "]["<<_log.name()<<"] ";
    _id = _key_string_stream.str();
    connect();
}

DadaClientBase::~DadaClientBase()
{
    if (_connected)
    {
        disconnect();
    }
}

std::size_t DadaClientBase::data_buffer_size() const
{
    return ipcbuf_get_bufsz((ipcbuf_t *) _hdu->data_block);
}

std::size_t DadaClientBase::header_buffer_size() const
{
    return ipcbuf_get_bufsz(_hdu->header_block);
}

std::size_t DadaClientBase::data_buffer_count() const
{
    return ipcbuf_get_nbufs((ipcbuf_t *) _hdu->data_block);
}

std::size_t DadaClientBase::header_buffer_count() const
{
    return ipcbuf_get_nbufs(_hdu->header_block);
}

void DadaClientBase::connect()
{
    PANDA_LOG_DEBUG << this->id() << "Connecting to dada buffer";
    _hdu = dada_hdu_create(_log.native_handle());
    dada_hdu_set_key(_hdu, _key);
    if (dada_hdu_connect (_hdu) < 0)
    {
        _log.write(LOG_ERR, "could not connect to hdu\n");
        throw panda::Error("Unable to connect to hdu");
    }
    PANDA_LOG_DEBUG << this->id() << "Header buffer is " << header_buffer_count()
                    << " x " << header_buffer_size() << " bytes";
    PANDA_LOG_DEBUG << this->id() << "Data buffer is " << data_buffer_count()
                    << " x " << data_buffer_size() << " bytes";
    _connected = true;
}

void DadaClientBase::disconnect()
{
    PANDA_LOG_DEBUG << this->id() << "Disconnecting from dada buffer";
    if (dada_hdu_disconnect (_hdu) < 0)
    {
        _log.write(LOG_ERR, "could not disconnect from hdu\n");
        throw panda::Error("Unable to disconnect from hdu");
    }
    dada_hdu_destroy(_hdu);
    _connected = false;
}

void DadaClientBase::reconnect()
{
    disconnect();
    connect();
}

std::string const& DadaClientBase::id() const
{
    return _id;
}

} // namespace psrdada
} // namespace cheetah
} // namespace ska
