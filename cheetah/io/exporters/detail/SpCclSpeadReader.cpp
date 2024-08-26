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

#include "SpeadLoggingAdapter.h"
#include <panda/Log.h>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::Stream::Stream(SpCclSpeadReader& reader, boost::asio::io_service& engine
                                                                      , DataFactory const& factory
                                                                      , Callback callback)

    : BaseT(engine, spead2::recv::stream_config().set_max_heaps(reader._max_heaps).set_allow_out_of_order(true))
    , _engine(engine)
    , _reader(reader)
    , _data_factory(factory)
    , _callback(callback)
{
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::Stream::Stream(Stream&& stream)
    : BaseT(stream._engine, spead2::recv::stream_config().set_max_heaps(stream._reader._max_heaps).set_allow_out_of_order(true))
    , _engine(stream._engine)
    , _reader(stream._reader)
    , _data_factory(std::move(stream._data_factory))
    , _callback(std::move(stream._callback))
{
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::Stream::~Stream()
{
    bool stopped = false;
    std::thread th([&, this]() { this->stop(); stopped=true; });
    while(!stopped) {
        _engine.poll_one();
    }
    th.join();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::Stream::heap_ready(spead2::recv::live_heap&& heap)
{
    if (heap.is_complete())
    {
        spead2::recv::heap hp(std::move(heap));
        if(hp.get_descriptors().size() == hp.get_items().size()) return;
        std::shared_ptr<SpCclType> data(_data_factory());
        SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::fill(*data, hp); // heap is destroyed in the recv::heap constructor
        _callback(data);
    }
    else {
        PANDA_LOG_WARN << "discarding incomplete/currupted SpCcl data heap";
    }
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::Stream::stop_received()
{
    BaseT::stop_received();
    get_io_service().post([this]() {
        _reader.stream_reset();
        });
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::Stream::process()
{
    _engine.poll_one();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::SpCclSpeadReader(SpCclSpeadReaderConfig const& config, Callback callback, DataFactory const& factory)
    : SpCclSpeadStreamTraits()
    , _destructor(false)
    , _max_heaps(spead2::recv::stream_config::default_max_heaps * 4)
    , _spead_stream(new Stream(*this, static_cast<boost::asio::io_service&>(config.engine()), factory, callback))
{
    // Defined endpoint here and moved emplace reader to start.
    _spead_stream_endpoint = config.endpoint();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::~SpCclSpeadReader()
{
    std::lock_guard<std::mutex> lk(_stop_mutex);
    _destructor = true;
    stop();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::stream_reset()
{
    std::unique_lock<std::mutex> lk(_stop_mutex, std::defer_lock_t());
    if(lk.try_lock() && !_destructor) {
        Stream* stream_ptr = nullptr;
        try {
            stream_ptr = new Stream(std::move(*_spead_stream));
            _spead_stream.reset(stream_ptr);
            visit_stop_listeners();
            PANDA_LOG << "Received end of SpCclSpead Stream - resetting";
        } catch(...) {
            delete stream_ptr;
            throw;
        }
        do_start();
    }
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::do_start()
{
    _spead_stream->template emplace_reader<spead2::recv::udp_reader>( _spead_stream_endpoint
                                                          , spead2::recv::udp_reader::default_max_size
                                                          //, 1024 * 1024 // socket buffer size
                                                          );
    visit_start_listeners();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::start()
{
    std::lock_guard<std::mutex> lk(_stop_mutex);
    do_start();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::add_stop_callback(std::function<void()> const& fn)
{
    _stop_listeners.push_back(fn);
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::add_start_callback(std::function<void()> const& fn)
{
    _start_listeners.push_back(fn);
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::visit_stop_listeners()
{
    for(auto const& fn: _stop_listeners) {
        fn();
    }
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::visit_start_listeners()
{
    for(auto const& fn: _start_listeners) {
        fn();
    }
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::stop()
{
    if(_spead_stream) _spead_stream->stop();
    visit_stop_listeners();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::process()
{
    _spead_stream->process();
}

template<typename TimeFrequencyT, typename Callback, typename DataFactory>
void SpCclSpeadReader<TimeFrequencyT, Callback, DataFactory>::fill(SpCclType& data, spead2::recv::heap const& heap)
{
    std::vector<spead2::recv::item> const& items = heap.get_items();
    if(items.size() == 0 ) return;

    //std::vector<spead2::descriptor> const& descriptors = heap.get_descriptors();
    FrequencyType fch1;
    FrequencyType foff;
    TimeType tsamp;
    data::DimensionSize<data::Frequency> number_of_channels(0);

    std::vector<spead2::recv::item const*> tf_items;
    std::vector<utils::ModifiedJulianClock::time_point> tf_start_time;
    std::vector<spead2::recv::item const*> ft_items;
    std::vector<utils::ModifiedJulianClock::time_point> ft_start_time;

    using pss::astrotypes::units::julian_day;
    typedef typename utils::ModifiedJulianClock::time_point time_point;
    typedef typename SpCclType::CandidateType::Dm Dm;
    typedef typename SpCclType::CandidateType::Width Width;
    typedef typename SpCclType::CandidateType::Sigma SigmaType;
    typedef typename SpCclType::CandidateType::MsecTimeType DurationType;
    std::vector<time_point> candidate_start_time;
    std::vector<Dm> candidate_dm;
    std::vector<Width> candidate_width;
    std::vector<SigmaType> candidate_sigma;
    std::vector<DurationType> candidate_duration;

    for (auto const& item : items)
    {
        switch(item.id)
        {
            case spead2::DESCRIPTOR_ID:
                break;
            case tf_data_id:
            {
                tf_items.push_back(&item);
                break;
            }
            case ft_data_id:
            {
                ft_items.push_back(&item);
                break;
            }
            case data_channel_1_id:
                fch1 = *(reinterpret_cast<double*>(item.ptr)) * boost::units::si::mega * boost::units::si::hertz;
                break;
            case data_channel_width_id:
                foff = *(reinterpret_cast<double*>(item.ptr)) * boost::units::si::mega * boost::units::si::hertz;
                break;
            case data_sample_interval_id:
                tsamp = *(reinterpret_cast<typename TimeType::value_type*>(item.ptr)) *  boost::units::si::second;
                break;
            case data_number_of_channels_id:
                number_of_channels = data::DimensionSize<data::Frequency>(*(reinterpret_cast<std::size_t*>(item.ptr)));
                break;
            case tf_data_start_time_id:
                tf_start_time.push_back(time_point(julian_day(*reinterpret_cast<double*>(item.ptr))));
                break;
            case ft_data_start_time_id:
                ft_start_time.push_back(time_point(julian_day(*reinterpret_cast<double*>(item.ptr))));
                break;
            case candidate_start_time_id:
                candidate_start_time.emplace_back(time_point(julian_day(*reinterpret_cast<double*>(item.ptr))));
                break;
            case candidate_dm_id:
                candidate_dm.emplace_back(*reinterpret_cast<typename Dm::value_type*>(item.ptr) * data::parsecs_per_cube_cm);
                break;
            case candidate_width_id:
                candidate_width.emplace_back(*reinterpret_cast<typename Width::value_type*>(item.ptr) * boost::units::si::milli * boost::units::si::second );
                break;
            case candidate_sigma_id:
                candidate_sigma.emplace_back(*reinterpret_cast<SigmaType*>(item.ptr));
                break;
            case candidate_duration_id:
                candidate_duration.emplace_back(*reinterpret_cast<typename DurationType::value_type*>(item.ptr) * boost::units::si::milli * boost::units::si::second);
                break;

            default:
                PANDA_LOG_WARN << "unknown id in spead packet detectedi: " << item.id;
        }
    }

    // load in tf data
    if(tf_items.size() != tf_start_time.size()) {
        PANDA_LOG_ERROR << "TimeFrequency data corrupted" << tf_items.size() << " vs " << tf_start_time.size();
    }

    data::DimensionSize<data::Time> number_of_spectra(0);
    const std::size_t sample_size=sizeof(NumericalRep);
    for( std::size_t i=0; i < tf_items.size(); ++i) {
        auto const& item = *tf_items[i];
        number_of_spectra += data::DimensionSize<data::Time>(item.length/sample_size/(std::size_t)number_of_channels);
    }
    std::shared_ptr<TimeFrequencyType> tf_block = std::make_shared<TimeFrequencyType>(number_of_spectra, number_of_channels);
    TimeFrequencyType& dat = *tf_block;
    dat.start_time(tf_start_time[0]);
    dat.sample_interval(tsamp);
    dat.set_channel_frequencies_const_width(fch1, foff);
    auto dat_it=dat.begin();

    // copy data into tf_block
    for( std::size_t i=0; i < tf_items.size(); ++i) {
        auto const& item = *tf_items[i];
        NumericalRep* start=reinterpret_cast<NumericalRep*>(item.ptr);
        std::size_t length = item.length/sizeof(NumericalRep);
        std::copy(start, start + length, dat_it);
        dat_it += length;
    }

    // load in ft data
    if(ft_items.size() !=0) {
        PANDA_LOG_WARN << "FrequencyTime data not yet supported";
        /*
        if(ft_items.size() != ft_start_time.size()) {
            PANDA_LOG_ERROR << "FrequencyTime data corrupted";
        }
        for( std::size_t i=0; i < ft_items.size(); ++i) {
            auto const& item = ft_items[i];
            data::DimensionSize<data::Frequency> number_of_channels = item.length/sample_size/(std::size_t)number_of_spectra;
            ft_blocks.emplace_back(new FrequencyTimeType(number_of_channels, number_of_spectra));
            FrequencyTimeType& dat = *ft_blocks.back();
            dat.start_time(ft_start_time[i]);
        }
        // convert to tf blocks and add to tf_blocks (sorted by timestamp)
        */
    }

    // load in candidates
    data::SpCandidateData<TimeFrequencyType> dt(tf_block);
    std::size_t total_candidates=candidate_dm.size();

    // sanity check
    if( total_candidates != candidate_width.size() ||
        total_candidates != candidate_start_time.size() ||
        total_candidates != candidate_sigma.size() ||
        total_candidates != candidate_duration.size() )
    {
        PANDA_LOG_ERROR << "candidate data corrupted";
        return;
    }

    for(std::size_t i=0; i< total_candidates; ++i)
    {
        typedef typename std::remove_reference<decltype(data)>::type::CandidateType SpCandidateType;
        dt.add(SpCandidateType(candidate_dm[i]
                                 , candidate_start_time[i]
                                 , candidate_width[i]
                                 , candidate_duration[i]
                                 , candidate_sigma[i]
                                 ));
    }

    // transfer the data to the passed object
    data = std::move(dt);
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
