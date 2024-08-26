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
#include "cheetah/generators/DispersedPulse.h"
#include <limits>
#include <algorithm>
#include <cmath>

namespace ska {
namespace cheetah {
namespace generators {


template<typename DataType>
DispersedPulse<DataType>::DispersedPulse()
    : BaseT()
    , _dm_constant(4.1493775933609e3) // s_mhz_squared_cm_cubed_per_pc
    , _dm_measure(500.0 * pss::astrotypes::units::parsecs_per_cube_cm)
    , _pulse_width(3.0 * boost::units::si::milli * boost::units::si::seconds)
    , _delta(1)
{
}

template<typename DataType>
DispersedPulse<DataType>::DispersedPulse(DispersedPulseConfig const& config)
    : BaseT()
    , _dm_constant(4.1493775933609e3) // s_mhz_squared_cm_cubed_per_pc
    , _dm_measure(config.dispersion_measure())
    , _pulse_width(config.pulse_width())
    , _delta(std::max(config.delta(), (double)std::numeric_limits<NumericalRep>::max()))
{
}

template<typename DataType>
DispersedPulse<DataType>::~DispersedPulse()
{
}

template<typename DataType>
void DispersedPulse<DataType>::next(DataType& data)
{
    std::size_t bin_width = _pulse_width/data.sample_interval();
    std::vector<data::FrequencyType> const& channel_freqs = data.channel_frequencies();
    typename data::FrequencyType freq_top = *std::max_element(channel_freqs.begin(), channel_freqs.end());
    std::vector<float> delays;
    for (auto const& freq: channel_freqs)
    {
        delays.push_back(_dm_constant * _dm_measure.value()
                        * (1.0/(freq.value()*freq.value()) -  1.0/(freq_top.value()*freq_top.value())) / data.sample_interval().value());
    }

    std::size_t const number_of_channels = data.template dimension<data::Frequency>();
    for(std::size_t ii=0; ii < number_of_channels; ++ii)
    {
        typename DataType::Channel ts = data.channel(ii);
        std::size_t delay = std::size_t(std::round(delays[ii]));

        // add signal to each appropraite bin
        for(std::size_t offset=0; offset < bin_width; ++offset) {
            //auto index = delay + offset;
            pss::astrotypes::DimensionIndex<pss::astrotypes::units::Time> const index(delay + offset);
            if( index >= ts.template size<data::Time>() ) continue;
            // avoid wrap-around when we add the delta
            if(std::numeric_limits<NumericalRep>::max() - ts[index] <= _delta)
            {
                ts[index] = std::numeric_limits<NumericalRep>::max();
            }
            else {
                ts[index] += _delta;
            }
        }
    }
}

} // namespace generators
} // namespace cheetah
} // namespace ska
