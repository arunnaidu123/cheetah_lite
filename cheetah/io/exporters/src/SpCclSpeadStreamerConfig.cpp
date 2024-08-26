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
#include "cheetah/io/exporters/SpCclSpeadStreamerConfig.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

SpCclSpeadStreamerConfig::SpCclSpeadStreamerConfig()
    : BaseT("sps_spead_stream")
    , _endpoint_config("ip")
    , _packet_size(4736)
    , _send_rate(1000000)
{
    _endpoint_config.address(ska::panda::IpAddress(9027, "127.0.0.1"));
    add(_endpoint_config);
    add(_window_config);
}

SpCclSpeadStreamerConfig::~SpCclSpeadStreamerConfig()
{
}

void SpCclSpeadStreamerConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
       ("packet_size",boost::program_options::value<unsigned>(&_packet_size)->default_value(_packet_size), "size of the UDP packet")
       ("rate_limit",boost::program_options::value<float>(&_send_rate)->default_value(_send_rate), "limit the UDP send rate to avoid dropped packets (0.0=nlimited)");
}

unsigned SpCclSpeadStreamerConfig::packet_size() const
{
    return _packet_size;
}

panda::IpAddress const& SpCclSpeadStreamerConfig::send_address() const
{
    return _endpoint_config.address();
}

data::CandidateWindowConfig const& SpCclSpeadStreamerConfig::window_config() const
{
    return _window_config;
}

void SpCclSpeadStreamerConfig::window_config( data::CandidateWindowConfig& window_config )
{
    _window_config = window_config;
}

float SpCclSpeadStreamerConfig::send_rate_limit() const
{
    return _send_rate;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
