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
#include "cheetah/generators/SimplePulsar.h"
#include "cheetah/generators/SimplePulsarConfig.h"
#include "cheetah/data/FrequencyTime.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace generators {

template<typename DataType>
std::random_device SimplePulsar<DataType>::_dev;

template<typename DataType>
SimplePulsar<DataType>::SimplePulsar(SimplePulsarConfig const& config)
    : BaseT()
    , _config(config)
    , _generator(_dev())
    , _rand(_config.mean(), _config.std_deviation())
    , _gaussian_pulse(0)
    , _start_sample_number(0)
    , _samples_per_period(0)
    , _dm_constant(data::dm_constant::s_mhz::dm_constant)
{

}

template<typename DataType>
SimplePulsar<DataType>::~SimplePulsar()
{
}

template<typename DataType>
void SimplePulsar<DataType>::next(DataType& data)
{
    std::vector<float> noise_data(data.number_of_channels()*data.number_of_spectra());
    std::transform(noise_data.begin(), noise_data.end(), noise_data.begin(), [this](float) { return _rand(_generator); } );
    if(_gaussian_pulse.size()==0) generate_pulse(data);
    add_pulsar(data, noise_data);

}

template<typename DataType>
float SimplePulsar<DataType>::mean()
{
    return _config.mean();
}

template<typename DataType>
long double SimplePulsar<DataType>::dmdelay(long double f1, long double f2, float dm, float tsamp)
{
  return(_dm_constant.value()*((1.0/f1/f1)-(1.0/f2/f2))*dm/tsamp);
}


template<typename DataType>
void SimplePulsar<DataType>::generate_pulse(DataType& data)
{
    unsigned int width_value = (unsigned)(_config.width().value()/data.sample_interval().value());
    float sigma = width_value/2.365;
    unsigned int number_of_channels = data.number_of_channels();
    float stddev = _config.std_deviation();
    double period = std::ceil(_config.period()/data.sample_interval())*data.sample_interval().value();
    _samples_per_period = period/data.sample_interval().value();
    _gaussian_pulse.resize(_samples_per_period);
    for(unsigned int i=0; i<_gaussian_pulse.size(); ++i)
    {
        _gaussian_pulse[i] = stddev*(_config.snr()/std::sqrt((float)(3.14*sigma*number_of_channels)))*std::exp(-0.5*std::pow(i-_samples_per_period/2.0,2)/(std::pow(sigma,2)));
    }
    _shifts.resize(number_of_channels);
    auto foff = (data.low_high_frequencies().second-data.low_high_frequencies().first).value()/number_of_channels;
    auto fch1 = data.low_high_frequencies().second;
    for(unsigned i=0; i<_shifts.size();++i)
    {
        long double f1 = (fch1.value()-i*foff);
        long double delay = dmdelay(f1,fch1.value(),_config.dm().value(),data.sample_interval().value());
        _shifts[i] = (unsigned)(delay);
    }
}

template<typename DataType>
typename data::EnableIfIsTimeFrequency<DataType, void> SimplePulsar<DataType>::add_pulsar(DataType& data, std::vector<float>& noise_data)
{
    for(unsigned int i=0; i<data.number_of_channels(); ++i)
    {

        auto noise_it = noise_data.begin()+data.number_of_spectra()*i;

        for(unsigned int sample=0; sample<data.number_of_spectra(); ++sample)
        {
            int phase = (_start_sample_number-_shifts[i]+sample)%_samples_per_period;
            if(phase<0) phase += _samples_per_period;
            if(phase==(int)_samples_per_period) phase=0;
            *noise_it = *noise_it + _gaussian_pulse[phase];
            ++noise_it;
        }
    }

    corner_turn::corner_turn( &*noise_data.begin()
                           , &*data.begin()
                           , data.number_of_spectra()
                           , data.number_of_channels()
                           );

    _start_sample_number += data.number_of_spectra();
}

} // namespace generators
} // namespace cheetah
} // namespace ska
