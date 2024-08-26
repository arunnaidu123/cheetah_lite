/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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

#include "cheetah/modules/ddtr/cpu/DdtrProcessor.h"
#include "cheetah/data/FrequencyTime.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

template<typename DdtrTraits>
DdtrProcessor<DdtrTraits>::DdtrProcessor(std::shared_ptr<DedispersionPlanType> plan
                                        , FrequencyTimeType& data
                                        , std::shared_ptr<DmTrialsType> dm_trials_ptr
                                        )
    : _plan(plan)
    , _ft_data(data)
    , _dm_trials_ptr(dm_trials_ptr)
    , _current_dm_range(0)
    , _current_dm_idx(0)
{
}

template<typename DdtrTraits>
bool DdtrProcessor<DdtrTraits>::finished() const
{
    return _current_dm_range >= _plan->algo_config().number_of_dms().size();
}

template<typename DdtrTraits>
std::size_t const& DdtrProcessor<DdtrTraits>::current_dm_range() const
{
    return _current_dm_range;
}

template<typename DdtrTraits>
void DdtrProcessor<DdtrTraits>::downsample_data()
{
    NumericalRep* ft_data = &*_ft_data.begin();

    for(unsigned channel=0; channel<_ft_data.number_of_channels(); ++channel)
    {
        for(unsigned sample=0; sample<_ft_data.number_of_spectra()/2; ++sample)
        {
            unsigned element_idx = channel*_ft_data.number_of_spectra()+2*sample;
            double temp = (double)(ft_data[element_idx]) + (double)(ft_data[element_idx+1]);
            temp /= 2;
            ft_data[channel*_ft_data.number_of_spectra()+sample] = (NumericalRep)temp;
        }
    }
}

template<typename DdtrTraits>
DdtrProcessor<DdtrTraits>& DdtrProcessor<DdtrTraits>::operator++()
{
    if(_current_dm_range!=0)
    {
        downsample_data();
    }

    DmTrialsType& dmtrials = *(_dm_trials_ptr);

    auto const& plan_dm_trials = _plan->algo_config().dm_trials();
    auto const& plan_dm_factors = _plan->dm_factors();

    for (std::size_t dm_idx = _current_dm_idx; dm_idx < _current_dm_idx+_plan->algo_config().number_of_dms()[_current_dm_range]; ++dm_idx)
    {
        auto& current_trial = dmtrials[dm_idx];
        std::fill(current_trial.begin(), current_trial.end(), 0);
        auto const& plan_dm_trial = plan_dm_trials[dm_idx].first.value();
        auto const& downsampling_factor = plan_dm_trials[dm_idx].second;
        for (std::size_t chan_idx=0; chan_idx < _ft_data.number_of_channels(); ++chan_idx)
        {
            std::size_t delay = static_cast<std::size_t>(plan_dm_factors[chan_idx] * plan_dm_trial/downsampling_factor);

            std::size_t shift = chan_idx*_ft_data.number_of_spectra();
            std::transform (current_trial.begin(), current_trial.end()
                           , _ft_data.begin()+shift+delay, current_trial.begin(), [&](float x, NumericalRep y){return ((float)x+y);});
        }
        std::transform(current_trial.begin(), current_trial.end(), current_trial.begin(), [&](float x){return x/_ft_data.number_of_channels();});
    }
    _current_dm_idx += _plan->algo_config().number_of_dms()[_current_dm_range];
    ++_current_dm_range;
    return *this;
}

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
