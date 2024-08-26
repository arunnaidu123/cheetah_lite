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
#include "cheetah/modules/sps/Config.h"
#include "cheetah/modules/ddtr/DedispersionConfig.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace sps {

Config::Config(ddtr::Config& ddtr_config)
    : BaseT("sps")
    , _ddtr_config(ddtr_config)
    , _threshold(10.0)
{
}

Config::operator ddtr::Config const& () const
{
    return _ddtr_config;
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("threshold", boost::program_options::value<float>(&_threshold), "single pulse detection threshold in sigmas");
}

float Config::threshold() const
{
    return _threshold;
}

cpu::Config const& Config::cpu_config() const
{
    return BaseT::config<cpu::Config>();
}

cpu::Config& Config::cpu_config()
{
    return BaseT::config<cpu::Config>();
}

klotski_bruteforce::Config const& Config::klotski_bruteforce_config() const
{
    return BaseT::config<klotski_bruteforce::Config>();
}

klotski_bruteforce::Config& Config::klotski_bruteforce_config()
{
    return BaseT::config<klotski_bruteforce::Config>();
}

klotski::Config const& Config::klotski_config() const
{
    return BaseT::config<klotski::Config>();
}

klotski::Config& Config::klotski_config()
{
    return BaseT::config<klotski::Config>();
}

ddtr::Config& Config::ddtr_config()
{
    return _ddtr_config;
}

ddtr::Config const& Config::ddtr_config() const
{
    return _ddtr_config;
}

std::size_t Config::dedispersion_samples() const
{
    return _ddtr_config.dedispersion_samples();
}

} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
