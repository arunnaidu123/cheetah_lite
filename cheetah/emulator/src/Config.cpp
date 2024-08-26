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
#include "cheetah/emulator/Config.h"
#include "panda/ConfigFile.h"


namespace ska {
namespace cheetah {
namespace emulator {

Config::Config()
    : cheetah::utils::Config("emulator")
    , _endpoint_config("client")
{
    _endpoint_config.address(ska::panda::IpAddress(34345, "127.0.0.1"));
    add(_generators_config);
    add(_ska_low_config);
    add(_ska_mid_config);
    add(_endpoint_config);
    add(_server_config);
}

Config::~Config()
{
}

panda::Engine& Config::engine() const
{
    return _server_config.engine();
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("channels", boost::program_options::value<std::size_t>(&_number_of_channels)->default_value(7776), "the number of channels to generate")
    ("generator", boost::program_options::value<std::string>(&_generator), "select the data generator to use in the emulation");
}

Config::EndPointType Config::fixed_end_point() const
{
    return _endpoint_config.address().end_point<EndPointType>();;
}

void Config::fixed_address(panda::IpAddress const& address)
{
    _endpoint_config.address(address);
}

cheetah::generators::Config const& Config::generators_config() const
{
    return _generators_config;
}

cheetah::io::producers::rcpt_low::PacketGeneratorConfig const& Config::ska_low_config() const
{
    return _ska_low_config;
}

cheetah::io::producers::rcpt_mid::PacketGeneratorConfig const& Config::ska_mid_config() const
{
    return _ska_mid_config;
}

panda::ServerConfig const& Config::server_config() const
{
    return _server_config;
}

std::size_t Config::number_of_channels() const
{
    return _number_of_channels;
}

void Config::number_of_channels(std::size_t n)
{
    _number_of_channels = n;
}

std::string const& Config::generator() const
{
    return _generator;
}

} // namespace emulator
} // namespace cheetah
} // namespace ska
