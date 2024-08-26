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
#include "cheetah/io/exporters/SpCclSpeadStreamer.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

SpCclSpeadStreamer::SpCclSpeadStreamer(SpCclSpeadStreamerConfig const& config, panda::Engine& engine)
    : _config(config)
    , _sp_flavour(spead2::maximum_version, 64, 48, spead2::BUG_COMPAT_PYSPEAD_0_5_2)
    , _sp_end(_sp_flavour)
    , _sp_stream(engine, config.send_address().end_point<boost::asio::ip::udp::endpoint>()
                , spead2::send::stream_config().set_max_packet_size(config.packet_size())
                                               .set_rate(config.send_rate_limit())
                                               .set_burst_size(spead2::send::stream_config::default_burst_size)
                                               .set_max_heaps(spead2::send::stream_config::default_max_heaps * 8)
                )
{
    std::string limit_msg;
    if(config.send_rate_limit()!=0.0) {
         limit_msg = " (limited to " + std::to_string(config.send_rate_limit()) + " bytes/sec)";
    };
    PANDA_LOG << "Spead UDP output stream on " << config.send_address().address().to_string()
              << ":" << config.send_address().port()
              << limit_msg;
    _sp_end.add_end();
    _tf_data_desc.id = tf_data_id;
    _tf_data_desc.name = "TimeFrequency";
    _tf_data_desc.description = "TimeFrequency Data In Spectra order";

    _ft_data_desc.id = tf_data_id;
    _ft_data_desc.name = "FrequencyTime";
    _ft_data_desc.description = "Frequency TimeSeries Data";

    _data_start_time_desc.id = candidate_start_time_id;
    _data_start_time_desc.name = "DataStartTime";
    _data_start_time_desc.description= "MJD of candidate block start";
    _data_start_time_desc.format.emplace_back('f', sizeof(double) * 8);

    _data_number_of_channels_desc.id = data_number_of_channels_id;
    _data_number_of_channels_desc.name = "channels";
    _data_number_of_channels_desc.description = "number of channels in the data block";
    _data_number_of_channels_desc.format.emplace_back('i', sizeof(std::size_t) * 8);

    _data_channel_1.id = data_channel_1_id;
    _data_channel_1.name = "top frequency";

    _data_channel_width.id = data_channel_width_id;
    _data_channel_width.name = "channel_width";

    _data_sammple_interval.id = data_sample_interval_id;
    _data_sammple_interval.name = "sample_interval";

    _candidate_start_time_desc.id = candidate_start_time_id;
    _candidate_start_time_desc.name = "CandidateStartTime";
    _candidate_start_time_desc.description= "MJD of candidate pulse start";
    _candidate_start_time_desc.format.emplace_back('f', sizeof(double) * 8);

    _candidate_dm_desc.id = candidate_dm_id;
    _candidate_dm_desc.name = "dm";
    _candidate_dm_desc.description = "DispersionMeasure";

    _candidate_width_desc.id = candidate_width_id;
    _candidate_width_desc.name = "width";
    _candidate_width_desc.description = "Pulse Width";

    _candidate_sigma_desc.id = candidate_sigma_id;
    _candidate_sigma_desc.name = "sigma";
    _candidate_sigma_desc.description = "Pulse Sigma";

    _candidate_duration_desc.id = candidate_duration_id;
    _candidate_duration_desc.name = "duration";
    _candidate_duration_desc.description = "Pulse Duration";

    typedef typename data::SpCcl<uint8_t>::SpCandidateType CandidateType;
    _candidate_dm_desc.format.push_back(TypeHelper<typename CandidateType::Dm::value_type>::format());
    _candidate_width_desc.format.push_back(TypeHelper<typename CandidateType::MsecTimeType::value_type>::format());
    _candidate_sigma_desc.format.push_back(TypeHelper<typename CandidateType::SigmaType>::format());
    _candidate_duration_desc.format.push_back(TypeHelper<typename CandidateType::MsecTimeType::value_type>::format());

}

SpCclSpeadStreamer::~SpCclSpeadStreamer()
{
    _sp_stream.async_send_heap(_sp_end, [] (const boost::system::error_code&, spead2::item_pointer_t /*bytes_transferred*/){});
    bool stopped = false;
    std::thread th([&]() { _sp_stream.flush(); stopped=true; });
    while(!stopped) {
        _sp_stream.get_io_service().poll_one();
    }
    th.join();
}


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
