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
#include "cheetah/psrdada/Config.h"
#include "panda/Error.h"
#include<sstream>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

// @brief describing the configuration for sigprocdada stream

key_t string_to_key(std::string const& in)
{
    key_t key;
    std::stringstream converter;
    converter << std::hex << in;
    if (converter.fail())
    {
         throw panda::Error("Wrong input parameter. The dada key has to be a hexadecimal number.");
    }
    converter >> key;
    return key;
}
} // naemspace detail

Config::Config()
    : cheetah::utils::Config("psrdada")
    , _dada_key(0xdada)
    , _number_of_samples(128)
{
}

Config::~Config()
{
}


#ifdef ENABLE_PSRDADA
void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("number_of_threads", boost::program_options::value<unsigned>()->default_value(1U)->notifier([&](unsigned v) { _engine_config = v; }) , "the number of threads to run the engine")
    ("number_of_samples", boost::program_options::value<int>(&_number_of_samples)->default_value(128), "the number of time samples in each chunk")
    ("dada_key", boost::program_options::value<std::string>()
        ->default_value("dada")
        ->notifier([&](std::string in)
        {

            _dada_key = detail::string_to_key(in);

        }), "DADA buffer key");
}

key_t Config::dada_key() const { return _dada_key; }

void Config::dada_key(key_t const& key){ _dada_key = key; }

int Config::number_of_samples() const { return _number_of_samples; }

void Config::number_of_samples(int const& samples){ _number_of_samples = samples; }

Config::Engine& Config::engine() const
{
    if(!_engine)
    {
        _engine.reset(new Engine(_engine_config));
    }
    return *_engine;
}

unsigned Config::number_of_threads() const
{
    return _engine_config.number_of_threads();
}
#endif // ENABLE_PSRDADA

} // namespace psrdada
} // namespace cheetah
} // namespace ska
