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
#ifndef SKA_CHEETAH_PSRDADA_DADAWRITECLIENT_H
#define SKA_CHEETAH_PSRDADA_DADAWRITECLIENT_H

#include "cheetah/psrdada/DadaClientBase.h"
#include "cheetah/psrdada/detail/RawBytes.h"
#include "cheetah/psrdada/detail/RawBytesWriter.h"
#include <memory>
#include <cstddef>

namespace ska {
namespace cheetah {
namespace psrdada {

/**
 * @brief      Class that provides means for writing to
 *             a DADA ring buffer
 *
 * @details     This class provides writer-like access to a ring of
 *             DADA HDU memory buffers.
 *
 * @code
 *             // To use the DadaWriteClient class, instantiate it with a shared memory key
 *             key_t key = 0xadeadbee;
 *             ...
 *             SigprocHeader header;
 *             ...
 *             DadaWriteClient client(key, [](std::ostream& out){header.write(out)});
 *
 *             // Write data
 *             std::vector<char> data(1024);
 *             client.write<char>(data.begin(), data.end());
 * @endcode
 *
 *
 */
class DadaWriteClient: public DadaClientBase
{
    typedef std::function<void(std::ostream&)> NewSequenceCallback;

    public:
        /**
         * @brief      Instatiate new DadaWriteClient
         *
         * @param[in]  key                    Hexidecimal key to DADA shared memory block
         * @param      new_sequence_callback  A callback to be called on the start of each new
         *                                    sequence in the DADA buffer.
         *
         * @details     Upon instantiaton the DadaWriteClient instance will connect to the DADA
         *             buffer identified by the key and open a header block for writing, this
         *             header block is passed to the new_sequence_callback in the form of a
         *             std::ostream reference.
         *
         *             To end writing for the current sequence and start a new sequence, use
         *             the new_sequence method.
         */
        DadaWriteClient(key_t key, NewSequenceCallback const& new_sequence_callback);
        DadaWriteClient(DadaWriteClient const&) = delete;
        ~DadaWriteClient();

        /**
         * @brief      Write to the data ring buffer
         *
         * @param      begin     An iterator pointing to the start of the sequence to write.
         * @param      end       An iterator pointing to the end of the sequence to write.
         *
         * @tparam     DataType  The data type to be written to the buffer.
         * @tparam     Iterator  The iterator type.
         *
         * @details     Data will be cast to DataType upon writing to the buffer. It is the
         *             responsibility of the caller to ensure that this cast does not result
         *             in under or overflow.
         */
        template <typename Iterator, typename DataType=typename std::iterator_traits<Iterator>::value_type>
        void write(Iterator& begin, Iterator const& end);

        /**
         * @brief      Start a new writing sequence in the buffer
         *
         * @details     This call will insert and end-of-data marker into the ring
         *             buffer and invoke the new_sequence_callback with a std::ostream
         *             reference into the next header block.
         */
        void new_sequence();

        /**
         * @brief:  Write an EOD marker without writing a next header
         */
        void write_eod();

    private:
        void new_sequence_impl();
        std::unique_ptr<detail::RawBytesWriter>& acquire_data_block();
        void release_data_block();
        void lock();
        void unlock();
        void reset();

    private:
        NewSequenceCallback _new_sequence_callback;
        std::unique_ptr<detail::RawBytes> _current_block;
        std::unique_ptr<detail::RawBytesWriter> _current_writer;
        bool _locked;
};

} // namespace psrdada
} // namespace cheetah
} // namespace ska

#include "cheetah/psrdada/detail/DadaWriteClient.cpp"

#endif // SKA_CHEETAH_PSRDADA_DADAWRITECLIENT_H
