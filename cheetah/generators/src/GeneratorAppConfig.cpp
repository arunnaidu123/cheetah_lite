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
#include "cheetah/generators/GeneratorAppConfig.h"
#include "cheetah/data/Units.h"
#include "cheetah/version.h"


namespace ska {
namespace cheetah {
namespace generators {


GeneratorAppConfig::GeneratorAppConfig(std::string const& app_name, std::string const& description)
    : BasicAppConfig(app_name, description)
    , _number_of_chunks(1)
    , _number_of_channels(1024)
    , _number_of_time_samples(4096)
    , _frequency(1200.0 * boost::units::si::mega * data::hz)
    , _channel_width(-300.0 * boost::units::si::kilo * data::hz)
    , _sample_interval(1.0 * boost::units::si::milli * boost::units::si::seconds)
{
    add(_generator_config);
    add(_sigproc_config);
}

GeneratorAppConfig::~GeneratorAppConfig()
{
}

ska::cheetah::generators::Config& GeneratorAppConfig::generator_config()
{
    return _generator_config;
}

sigproc::WriterConfig const& GeneratorAppConfig::sigproc_config() const
{
    return _sigproc_config;
}

std::string GeneratorAppConfig::version() const
{
    return std::string(cheetah::version) + "\n" + BasicAppConfig::version();
}

void GeneratorAppConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("list-generators", boost::program_options::bool_switch()->notifier([this](bool b)
                        {
                            if(b) {
                                for(auto const& gen : _generator_keys) {
                                    std::cout << "\t" << gen << "\n";
                                }
                            }
                            return false;
                        }), "display a list of generators available")
    ("generator,g", boost::program_options::value<std::vector<std::string>>(&_generator_selected)
                , "the name of the generator to use as a data model (see --list-generators)")
    ("channels", boost::program_options::value<std::size_t>(&_number_of_channels)->default_value(_number_of_channels)
               , "the number of channels in each block of data")
    ("samples", boost::program_options::value<std::size_t>(&_number_of_time_samples)->default_value(_number_of_time_samples)
              , "the number of time samples in each block of data")
    ("chunks", boost::program_options::value<std::size_t>(&_number_of_chunks)->default_value(_number_of_chunks)
             , "the number of block of data to generate")
    ("start_freq", boost::program_options::value<typename FrequencyType::value_type>()->default_value(_frequency.value())
                   ->notifier([this](typename FrequencyType::value_type const& val)
                              {
                                _frequency = val * boost::units::si::mega * data::hertz;
                              })
                 , "the frequency of the first channel (MHz)")
    ("channel_width", boost::program_options::value<typename FrequencyType::value_type>()->default_value(_channel_width.value())
                   ->notifier([this](typename FrequencyType::value_type const& val)
                              {
                                _channel_width = val * boost::units::si::mega * data::hertz;
                              })
                    , "the width of each channel (MHz)")
    ("sample_interval", boost::program_options::value<typename IntervalType::value_type>()->default_value(_sample_interval.value())
                   ->notifier([this](typename IntervalType::value_type const& val)
                              {
                                _sample_interval= val * boost::units::si::seconds;
                              })
                    , "the width of each channel (MHz)");
}

void GeneratorAppConfig::set_generator_list(std::vector<std::string> const& generator_names)
{
    _generator_keys = generator_names;
}

std::vector<std::string> const& GeneratorAppConfig::generator_list() const
{
    return _generator_keys;
}

std::vector<std::string> const& GeneratorAppConfig::data_generator() const
{
    return _generator_selected;
}

std::size_t GeneratorAppConfig::number_of_chunks() const
{
    return _number_of_chunks;
}

std::size_t GeneratorAppConfig::number_of_time_samples() const
{
    return _number_of_time_samples;
}

std::size_t GeneratorAppConfig::number_of_channels() const
{
    return _number_of_channels;
}

typename GeneratorAppConfig::FrequencyType GeneratorAppConfig::start_frequency() const
{
    return _frequency;
}

typename GeneratorAppConfig::FrequencyType GeneratorAppConfig::channel_width() const
{
    return _channel_width;
}

typename GeneratorAppConfig::IntervalType GeneratorAppConfig::sample_interval() const
{
    return _sample_interval;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
