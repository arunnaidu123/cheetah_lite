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
#include "cheetah/generators/DispersedPulseConfig.h"
#include "panda/BoostQuantityOption.h"

namespace ska {
namespace cheetah {
namespace generators {


DispersedPulseConfig::DispersedPulseConfig(std::string const& tag_name)
    : BaseT(tag_name)
    , _dispersion_measure(500.0 * pss::astrotypes::units::parsecs_per_cube_cm)
    , _delta(1.0)
    , _pulse_width(3.0 * boost::units::si::milli * boost::units::si::seconds)
{
}

DispersedPulseConfig::~DispersedPulseConfig()
{
}

pss::astrotypes::units::DispersionMeasure<double> DispersedPulseConfig::dispersion_measure() const
{
    return _dispersion_measure;
}

double DispersedPulseConfig::delta() const
{
    return _delta;
}

DispersedPulseConfig::TimeType DispersedPulseConfig::pulse_width() const
{
    return _pulse_width;
}

void DispersedPulseConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("dm", boost::program_options::value<decltype(_dispersion_measure)>(&_dispersion_measure)->default_value(_dispersion_measure)
         , "dispersion measure of the pulse")
    ("pulse_width", boost::program_options::value<TimeType>(&_pulse_width)->default_value(_pulse_width)
                  , "the width of the pulse (in seconds)")
    ("signal_delta", boost::program_options::value<double>(&_delta)->default_value(_delta)
                   , "signal strength above the background");
}

} // namespace generators
} // namespace cheetah
} // namespace ska
