/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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
#include "cheetah/modules/ddtr/klotski_bruteforce/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {


Config::Config()
    : utils::Config("klotski_bruteforce")
    , _active(false)
    , _cache_size(1024*1024)
    , _max_channels_per_klotski_bruteforce(256)
{
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("active"
    , boost::program_options::value<bool>(&_active)->default_value(_active)
    , "use this algorithm for dedispersion")
    ("cache_size"
    , boost::program_options::value<std::size_t>(&_cache_size)->default_value(_cache_size)
    , "cache size per core available for the klotski_bruteforce")
    ("max_channels_per_klotski_bruteforce"
    , boost::program_options::value<std::size_t>(&_max_channels_per_klotski_bruteforce)->default_value(_max_channels_per_klotski_bruteforce)
    , "upper limit on number of channels to use per klotski_bruteforce");
}

bool Config::active() const
{
    return _active;
}

void Config::active(bool state)
{
    _active = state;
}

std::size_t Config::max_channels_per_klotski_bruteforce() const
{
    return _max_channels_per_klotski_bruteforce;
}

void Config::max_channels_per_klotski_bruteforce(unsigned int value)
{
    _max_channels_per_klotski_bruteforce = value;
}

std::size_t Config::cache_size() const
{
    return _cache_size;
}

void Config::cache_size(unsigned int value)
{
    _cache_size = value;
}

} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
