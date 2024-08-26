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
#include "cheetah/modules/rfim/ampp/BandPassConfig.h"
#include <boost/units/systems/si/frequency.hpp>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace ampp {


BandPassConfig::BandPassConfig()
    : utils::Config("bandpass")
    , _mean(0.0)
{
}

BandPassConfig::~BandPassConfig()
{
}

void BandPassConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("mean", boost::program_options::value<SignalType>(&_mean)->default_value(_mean), "the mean set point of the bandpass");
}

BandPassConfig::SignalType BandPassConfig::mean() const
{
    return _mean;
}

void BandPassConfig::mean(BandPassConfig::SignalType mean)
{
    _mean = mean;
}

} // namespace ampp
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
