/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2024 The SKA organisation
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
#include "cheetah/generators/SimplePulsarConfig.h"


namespace ska {
namespace cheetah {
namespace generators {


SimplePulsarConfig::SimplePulsarConfig()
    : cheetah::utils::Config("gaussian_noise")
    , _mean(96.0)
    , _deviation(24.0)
    , _dm(50.0*data::parsecs_per_cube_cm)
    , _period(1.0*data::seconds)
    , _width(0.25*_period)
    , _snr(1.0)
{
}

SimplePulsarConfig::~SimplePulsarConfig()
{
}

void SimplePulsarConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("mean", boost::program_options::value<float>(&_mean)->default_value(_mean), "The mean value of the distribution")
    ("dev", boost::program_options::value<float>(&_deviation)->default_value(_deviation), "The deviation of the distribution")
    ("dm", boost::program_options::value<double>()->default_value(_dm.value())->notifier([this](double v) { _dm = v*data::parsecs_per_cube_cm; }), "The Dispersion measure of the pulsar pc/cm3")
    ("period", boost::program_options::value<float>()->default_value(_period.value())->notifier([this](float v) { _period = v*data::seconds; }), "The period of the pulsar in seconds")
    ("width", boost::program_options::value<float>()->default_value(_width.value())->notifier([this](float v) { _width = v*data::seconds; }), "The width of the pulse in seconds")
    ("snr", boost::program_options::value<float>(&_snr)->default_value(_snr), "Signal to noise per pulse");
}

float SimplePulsarConfig::mean() const
{
    return _mean;
}

void SimplePulsarConfig::mean(float val)
{
    _mean = val;
}

float SimplePulsarConfig::std_deviation() const
{
    return _deviation;
}

void SimplePulsarConfig::std_deviation(float val)
{
    _deviation = val;
}

SimplePulsarConfig::TimeType SimplePulsarConfig::period() const
{
    return _period;
}

void SimplePulsarConfig::period(TimeType val)
{
    _period = val;
}

SimplePulsarConfig::TimeType SimplePulsarConfig::width() const
{
    return _width;
}

void SimplePulsarConfig::width(TimeType val)
{
    _width = val;
}

SimplePulsarConfig::DmType SimplePulsarConfig::dm() const
{
    return _dm;
}

void SimplePulsarConfig::dm(DmType val)
{
    _dm = val;
}

float SimplePulsarConfig::snr() const
{
    return _snr;
}

void SimplePulsarConfig::snr(float val)
{
    _snr = val;
}

} // namespace generators
} // namespace cheetah
} // namespace ska