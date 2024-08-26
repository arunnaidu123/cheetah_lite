/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#include "cheetah/modules/ddtr/Config.h"
#include <algorithm>
#include <cmath>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

static std::string const dedispersion_tag("dedispersion");

Config::Config()
    : BaseT("ddtr")
{
    add(_rfi_excision_config);
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    BaseT::add_options(add_options);
}

cpu::Config const& Config::cpu_algo_config() const
{
    return BaseT::config<cpu::Config>();
}

cpu::Config& Config::cpu_algo_config()
{
    return BaseT::config<cpu::Config>();
}

klotski::Config const& Config::klotski_algo_config() const
{
    return BaseT::config<klotski::Config>();
}

klotski::Config& Config::klotski_algo_config()
{
    return BaseT::config<klotski::Config>();
}

klotski_bruteforce::Config const& Config::klotski_bruteforce_algo_config() const
{
    return BaseT::config<klotski_bruteforce::Config>();
}

klotski_bruteforce::Config& Config::klotski_bruteforce_algo_config()
{
    return BaseT::config<klotski_bruteforce::Config>();
}

RfiExcisionConfig const& Config::rfiexcision_config() const
{
    return _rfi_excision_config;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
