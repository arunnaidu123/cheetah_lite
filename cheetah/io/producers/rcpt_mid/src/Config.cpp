/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#include "cheetah/io/producers/rcpt_mid/Config.h"
#include "ska/cbf_psr_interface/CbfPsrPacket.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {


Config::Config()
    : utils::Config("udp_mid")
    , _engine_config(2)
    , _number_of_channels(3700U)
    , _spectra_per_chunk(1024U)
    , _endpoint_config("listen")
{
    _endpoint_config.address(ska::panda::IpAddress(34345, "127.0.0.1"));
    add(_endpoint_config);
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("number_of_threads", boost::program_options::value<unsigned>()->default_value(1U)->notifier([&](unsigned v) { _engine_config = v; }) , "the number of threads to run the engine")
    ("spectra_per_chunk", boost::program_options::value<unsigned>(&_spectra_per_chunk)->default_value(_spectra_per_chunk), "the number of time slices in each chunk (time_slices x no_of_channels = total data samples)")
    ("number_of_channels", boost::program_options::value<unsigned>(&_number_of_channels)->default_value(_number_of_channels), "the number of frequency channels in each time sample")
    ("max_buffers", boost::program_options::value<std::size_t>(&_max_buffer_count)->default_value(10U), "the max number of udp packet buffers to use");
}

Config::Engine& Config::engine() const
{
    if(!_engine) {
        _engine.reset(new Engine(_engine_config));
    }
    return *_engine;
}

boost::asio::ip::udp::endpoint Config::remote_end_point() const
{
    return _endpoint_config.address().end_point<boost::asio::ip::udp::endpoint>();;
}

void Config::remote_end_point(boost::asio::ip::udp::endpoint const& endpoint)
{
    _endpoint_config.address(ska::panda::IpAddress(endpoint));
}

unsigned Config::spectra_per_chunk() const
{
    return _spectra_per_chunk;
}

void Config::spectra_per_chunk(unsigned num)
{
    _spectra_per_chunk = num;
}

void Config::number_of_channels(unsigned num)
{
    _number_of_channels = num;
}

unsigned Config::number_of_channels() const
{
    return _number_of_channels;
}

std::size_t Config::max_buffers() const
{
    return _max_buffer_count;
}

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
