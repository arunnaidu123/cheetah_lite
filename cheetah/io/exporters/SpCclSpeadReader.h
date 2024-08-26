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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADREADER_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADREADER_H

#ifdef ENABLE_SPEAD

#include "detail/SpCclSpeadStreamTraits.h"
#include "SpCclSpeadReaderConfig.h"
#include "cheetah/data/SpCandidateData.h"
#include "pss/astrotypes/utils/TypeTraits.h"
#include <spead2/common_defines.h>
#include <spead2/recv_heap.h>
#include <spead2/recv_udp.h>
#include <functional>
#include <memory>
#include <vector>
#include <mutex>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief read in data sent from the SpCclSpeadStreamer
 * @details
 * @tparam Callback the function to be called with a full shared_ptr<SpCandidateData> class
 *      e.g. std::function<void(std::shared_ptr<data::SpCandidateData<TimeFrequencyType>> const&)>
 * @tparam DataFactory the function to be called when data has been read
 */

template<typename TimeFrequencyT, typename Callback=std::function<void(std::shared_ptr<data::SpCandidateData<TimeFrequencyT>>)>
                                , typename DataFactory=std::function<data::SpCandidateData<TimeFrequencyT>*()>>
class SpCclSpeadReader : public SpCclSpeadStreamTraits
{
    private:
        typedef TimeFrequencyT TimeFrequencyType;
        typedef data::SpCandidateData<TimeFrequencyType> SpCclType;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef typename TimeFrequencyType::TimeType TimeType;
        typedef typename TimeFrequencyType::DataType NumericalRep;

        class Stream : public spead2::recv::stream
        {
                typedef spead2::recv::stream BaseT;

            public:
                Stream(SpCclSpeadReader& reader, boost::asio::io_service& engine, DataFactory const& factory, Callback callback);
                Stream(Stream const&) = delete;
                Stream(Stream&&);
                ~Stream();
                void heap_ready(spead2::recv::live_heap &&) override;
                void stop_received() override;

                void process();

            private:
                boost::asio::io_service& _engine;
                SpCclSpeadReader& _reader;
                DataFactory _data_factory;
                Callback _callback;
        };

    public:
        SpCclSpeadReader(SpCclSpeadReaderConfig const&, Callback callback, DataFactory const& factory=[]() { return new SpCclType(); });
        ~SpCclSpeadReader();
        SpCclSpeadReader(SpCclSpeadReader const&) = delete;

        /**
         * @brief start processing heaps on the stream
         */
         void start();

        /**
         * @brief stop processing heaps on the stream
         */
         void stop();

        /**
         * @brief fill the SpCcl data structure from the data in the provided heap
         */
        static void fill(SpCclType& data, spead2::recv::heap const&);

        /**
         * @brief process a single event on the event queue
         * @details will call poll_one on the driving boost::asio::io_service engine.
         */
        void process();

        /**
         * @brief set a callback for whenever the reader is stopped
         */
        void add_stop_callback(std::function<void()> const& fn);

        /**
         * @brief set a callback for whenever the reader is started
         */
        void add_start_callback(std::function<void()> const& fn);

    private:
        void stream_reset();
        void do_start();
        void visit_stop_listeners();
        void visit_start_listeners();

    private:
        std::mutex _stop_mutex;
        bool _destructor;
        std::size_t _max_heaps;
        std::unique_ptr<Stream> _spead_stream;
        boost::asio::ip::udp::endpoint _spead_stream_endpoint;
        std::vector<std::function<void()>> _stop_listeners;
        std::vector<std::function<void()>> _start_listeners;
};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
#include "detail/SpCclSpeadReader.cpp"
#endif // ENABLE_SPEAD

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADREADER_H
