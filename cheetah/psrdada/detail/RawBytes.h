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
#ifndef SKA_CHEETAH_PSRDADA_RAWBYTES_H
#define SKA_CHEETAH_PSRDADA_RAWBYTES_H

#include <cstddef>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace details {

/**
 * @brief      Class for wrapping a raw pointer to a buffer of shared memory
 *
 * @details     This class is used to wrap pointers to shared memory
 *             returned by calls to the lower-level DADA API.
 *
 *             This class is used to wrap buffers acquired by both reading
 *             and writing clients. For writing clients, it is necessary to
 *             set the number of bytes written using the used_bytes() method
 *             after writing. This value is used when releasing the buffer.
 */
class RawBytes
{

    public:
        /**
         * @brief      Create a new RawBytes instance
         *
         * @param      ptr            The pointer to the buffer to wrap
         * @param[in]  total          The total number of bytes in the buffer
         * @param[in]  used           The number of bytes currently used in the buffer
         */
        RawBytes(char * ptr, std::size_t total, std::size_t used=0);
        RawBytes(RawBytes const&) = delete;
        ~RawBytes();

        /**
         * @brief      Get the total number of bytes in the buffer
         */
        std::size_t total_bytes() const;

        /**
         * @brief      Get the number of currently used bytes in the buffer
         */
        std::size_t used_bytes() const;

        /**
         * @brief      Set the number of currently used bytes in the buffer
         *
         * @details     For writing clients, this method should be called after
         *             all writes are complete so that the number of used_bytes
         *             can be passed to reading clients.
         */
        void used_bytes(std::size_t);

        /**
         * @brief      Get a raw pointer to the start of the buffer
         */
        char * ptr() const;

    private:
        char * _ptr;
        std::size_t _total_bytes;
        std::size_t _used_bytes;
};

} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_PSRDADA_RAWBYTES_H
