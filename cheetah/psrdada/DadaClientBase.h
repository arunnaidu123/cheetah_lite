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
#ifndef SKA_CHEETAH_PSRDADA_DADACLIENT_H
#define SKA_CHEETAH_PSRDADA_DADACLIENT_H

#include "cheetah/psrdada/detail/MultiLog.h"
#include "dada_hdu.h"
#include <string>
#include <sys/types.h>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

struct membuf : std::streambuf
{
    membuf(char* begin, char* end);
    char* reader_position() const;
    char* writer_position() const;
};

} // namespace detail

/**
 * @brief     A general base class for DADA readers and writers
 */

class DadaClientBase
{
    public:
        /**
         * @brief      Create a new basic DADA client instance
         *
         * @param[in]  key           The hexidecimal shared memory key
         * @param      logger_name   A name to give the internal multilog instance
         */
        DadaClientBase(key_t key, std::string const& logger_name);
        DadaClientBase(DadaClientBase const&) = delete;
        ~DadaClientBase();

        /**
         * @brief      Get the sizes of each data block in the ring buffer
         */
        std::size_t data_buffer_size() const;

        /**
         * @brief      Get the sizes of each header block in the ring buffer
         */
        std::size_t header_buffer_size() const;

        /**
         * @brief      Get the number of data blocks in the ring buffer
         */
        std::size_t data_buffer_count() const;

        /**
         * @brief      Get the number of header blocks in the ring buffer
         */
        std::size_t header_buffer_count() const;

        /**
         * @brief      Connect to ring buffer
         */
        void connect();

        /**
         * @brief      Disconnect from ring buffer
         */
        void disconnect();

        /**
         * @brief      Reconnect to the ring buffer
         */
        void reconnect();

        /**
         * @brief      Return a string identifier based on the buffer key and log name
         */
        std::string const& id() const;

    protected:
        key_t _key;
        dada_hdu_t* _hdu;
        bool _connected;
        detail::MultiLog _log;
        std::string _id;
};


} // namespace psrdada
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_PSRDADA_DADACLIENT_H
