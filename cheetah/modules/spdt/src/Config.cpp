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
#include "cheetah/modules/spdt/Config.h"
#include "cheetah/modules/ddtr/DedispersionConfig.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {

Config::Config()
    : BaseT("spdt")
    , _threshold(10.0)
{
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

} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
