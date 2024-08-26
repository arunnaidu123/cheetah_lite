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

#ifndef SKA_CHEETAH_PSRDADA_DADAREADCLIENT_H
#define SKA_CHEETAH_PSRDADA_DADAREADCLIENT_H

#include "cheetah/psrdada/DadaClientBase.h"
#include "cheetah/psrdada/detail/RawBytes.h"
#include "cheetah/psrdada/detail/RawBytesReader.h"
#include "panda/Engine.h"
#include <memory>
#include <cstddef>


namespace ska {
namespace cheetah {
namespace psrdada {
/**
 * @brief      Class that provides means for reading from
 *             a DADA ring buffer
 *
 * @details     This class provides reader-like access to a ring of
 *             DADA HDU memory buffers.
 *
 * @code
 *             // To use the DadaReadClient class, instantiate it with a shared memory key
 *             key_t key = 0xadeadbee;
 *             ...
 *             SigprocHeader header;
 *             ...
 *             DadaReadClient client(key, [](std::istream& in){header.read(in)});
 *
 *             // Read data
 *             std::vector<char> data(1024);
 *             client.read<char>(data.begin(), data.end());
 * @endcode
 *
 *
 */
class DadaReadClient: public DadaClientBase
{
    typedef std::function<void(std::istream&, std::exception_ptr)> NextSequenceCallback;

    public:
        /**
         * @brief      Instatiate new DadaReadClient
         *
         * @param[in]  key                     Hexidecimal key to DADA shared memory block
         * @param      next_sequence_callback  A callback to be called on the start of each
         *                                     subsequent sequence in the DADA buffer.
         *
         * @details     Upon instantiaton the DadaReadClient instance will connect to the DADA
         *             buffer identified by the key and open a header block for reading, this
         *             header block is passed to the next_sequence_callback in the form of a
         *             std::istream reference.
         *
         *             To end reading from the current sequence and start the next sequence, use
         *             the next_sequence method. This will flush all data in the buffer until an
         *             end-of-data marker is encountered, whereupon the next header block will be
         *             opened and passed to the next_sequence_callback.
         */
        DadaReadClient(key_t key, panda::Engine& engine,NextSequenceCallback const& next_sequence_callback);
        DadaReadClient(DadaReadClient const&) = delete;
        ~DadaReadClient();

        /**
         * @brief      Read from the data ring buffer
         *
         * @param      begin     An iterator pointing to the start of the sequence to read into.
         * @param      end       An iterator pointing to the end of the sequence to read into.
         *
         * @tparam     DataType  The data type to be read from the buffer.
         * @tparam     Iterator  The iterator type.
         *
         * @details     Data will be cast to DataType upon reading from the buffer. It is the
         *             responsibility of the caller to ensure that this cast does not result
         *             in under or overflow.
         */
        template <typename Iterator, typename DataType=typename std::iterator_traits<Iterator>::value_type>
        Iterator& read(Iterator& begin, Iterator const& end);

        /**
         * @brief      Move to the next sequence in the ring buffer.
         *
         * @details     This will flush all data in the buffer until an end-of-data marker is
         *             encountered, whereupon the next header block will be opened and passed
         *             to the default next_sequence_callback as passed in the constructor.
         */
        void next_sequence();


        /**
         * @brief      Move to the next sequence in the ring buffer.
         * @details    same as next_sequence() but will call the provided callback
         *             when a buffer becomes availabel instead of the default callback
         */
        template <typename CallBackFunctor>
        void next_sequence(CallBackFunctor callback);

        /*
         *  @brief stops the read client stream
         */
        void stop();

        /*
         * @brief returns the state of the stream
         */
        inline std::shared_ptr<bool> stopped() const;

        /*
         * @brief: start the read client stream
         */
        void start();  

    protected:
        template <typename CallBack>
        void do_next_sequence(std::shared_ptr<bool> destructor_flag, CallBack callback);

    private:
        std::unique_ptr<detail::RawBytesReader>& acquire_data_block();
        void release_data_block();
        void flush();
        void lock();
        void unlock();
        void reset();
        bool eod() const;
        char* open_block_read(ipcio_t* ipc, std::uint64_t *cursz, std::uint64_t *blk_id);
        char* ipcbuf_get_next_read (ipcbuf_t* id, uint64_t* bytes);

    private:
        NextSequenceCallback  _next_sequence_callback;
        std::unique_ptr<detail::RawBytes> _current_block;
        std::unique_ptr<detail::RawBytesReader> _current_reader;
        bool _locked;
        panda::Engine& _engine;
        mutable std::shared_ptr<bool>  _destructor_flag;
        std::mutex _block_mutex;
};

} // namespace psrdada
} // namespace cheetah
} // namespace ska

#include "cheetah/psrdada/detail/DadaReadClient.cpp"

#endif // SKA_CHEETAH_PSRDADA_DADAREADCLIENT_H
