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
#include "cheetah/generators/Config.h"


namespace ska {
namespace cheetah {
namespace generators {


Config::Config()
    : cheetah::utils::Config("generators")
{
    add(_gaussian_noise);
    //add(_baseband_gaussian_noise);
    add(_dispersed_pulse);
    add_factory("pulsar_injection", [](){ return new PulsarInjectionConfig(); });
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit&)
{
}

GaussianNoiseConfig const& Config::gaussian_noise() const
{
    return _gaussian_noise;
}

BasebandGaussianNoiseConfig const& Config::baseband_gaussian_noise() const
{
    return _baseband_gaussian_noise;
}

DispersedPulseConfig const& Config::dispersed_pulse() const
{
    return _dispersed_pulse;
}

Config::PulsarInjectionIterartorType Config::pulsar_injection_begin() const
{
    return PulsarInjectionIterartorType(subsection("pulsar_injection"));
}

Config::PulsarInjectionIterartorType Config::pulsar_injection_end() const
{
    return PulsarInjectionIterartorType(subsection_end());
}

} // namespace generators
} // namespace cheetah
} // namespace ska
