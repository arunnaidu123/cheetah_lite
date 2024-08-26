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


namespace ska {
namespace cheetah {
namespace emulator {

template<typename StreamFactoryT>
EmulatorAppConfig::EmulatorAppConfig(std::string const& app_name
                                   , std::string const& description
                                   , StreamFactoryT stream_factory)
    : BasicAppConfig(app_name, description)
    , _available_streams(stream_factory.available())
    , _generator_types_callback([stream_factory, this](EmulatorAppConfig const& c)
                                {
                                    stream_factory.print_generators(_stream_type, c.emulator_config());
                                }
                               )
{
    if(!_available_streams.empty())
        _stream_type = _available_streams[0];
    add(_emulator_config);
}

} // namespace emulator
} // namespace cheetah
} // namespace ska
