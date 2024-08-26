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
#include "cheetah/emulator/EmulatorAppConfig.h"
#include "cheetah/version.h"


namespace ska {
namespace cheetah {
namespace emulator {

EmulatorAppConfig::~EmulatorAppConfig()
{
}

ska::cheetah::emulator::Config& EmulatorAppConfig::emulator_config()
{
    return _emulator_config;
}

ska::cheetah::emulator::Config const& EmulatorAppConfig::emulator_config() const
{
    return _emulator_config;
}

std::string EmulatorAppConfig::version() const
{
    return std::string(cheetah::version) + "\n" + BasicAppConfig::version();
}

void EmulatorAppConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("list-streams", boost::program_options::bool_switch()->notifier(
                        [this](bool b) {
                            if(!b) return;
                            for( auto const& stream : _available_streams)
                            {
                                std::cout << stream << "\n";
                            }
                        }
                     ))
    ("stream", boost::program_options::value<std::string>()->default_value(_stream_type)
                            ->notifier([&, this](std::string const& value)
                                       {
                                           auto const& available = _available_streams;
                                           auto it = std::find(available.begin(), available.end(), value);
                                           if(it == available.end()) {
                                               throw panda::Error("unknown stream type");
                                           }
                                       }
                                      ), "type of UDP stream to generate")
    ("list-generators", boost::program_options::bool_switch()->notifier(
                        [this](bool b)
                        {
                            if(b) {
                                _generator_types_callback(*this);
                            }
                            return false;
                        }), "display a list of generators available");
}

std::string const& EmulatorAppConfig::stream_type() const
{
    return _stream_type;
}

} // namespace emulator
} // namespace cheetah
} // namespace ska
