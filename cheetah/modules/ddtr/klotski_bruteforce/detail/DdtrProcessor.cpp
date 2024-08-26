/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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

#include "cheetah/modules/ddtr/klotski_bruteforce/DdtrProcessor.h"
#include "cheetah/data/FrequencyTime.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {

void threaded_dedispersion(std::vector<std::vector<unsigned int>> const& channels_per_band
                       , std::size_t nsamps
                       , std::size_t dsamps
                       , std::size_t NDMS
                       , std::vector<float> const& dm_shifts
                       , std::vector<unsigned int> const& start_dm_shifts
                       , std::vector<unsigned short>& data_in
                       , std::vector<std::vector<int>>& data_temp
                       );

void integrate_klotski_bruteforce( float* data_out
                      , std::vector<std::vector<int>>& data_temp
                      , std::size_t number_of_channels
                      , std::size_t number_of_elements
                      , std::size_t offset
                      );

template<typename DdtrTraits>
DdtrProcessor<DdtrTraits>::DdtrProcessor(std::shared_ptr<DedispersionPlanType> plan
                                        , std::shared_ptr<DmTrialsType> dm_trials_ptr
                            )
    : _plan(plan)
    , _dm_trials_ptr(dm_trials_ptr)
    , _current_dm_range(0)
    , _total_number_of_dms(0)
{
}

template<typename DdtrTraits>
bool DdtrProcessor<DdtrTraits>::finished() const
{
    return _current_dm_range >= _plan->dedispersion_strategy()->number_of_dm_ranges();
}

template<typename DdtrTraits>
std::shared_ptr<typename DdtrProcessor<DdtrTraits>::DedispersionPlanType> DdtrProcessor<DdtrTraits>::dedispersion_plan()
{
    return _plan;
}

template<typename DdtrTraits>
std::size_t const& DdtrProcessor<DdtrTraits>::current_dm_range() const
{
    return _current_dm_range;
}

template<typename DdtrTraits>
DdtrProcessor<DdtrTraits>& DdtrProcessor<DdtrTraits>::operator++()
{
    unsigned int downsample_factor = std::pow(2,_current_dm_range);

    std::vector<float> dm_shifts(_plan->dedispersion_strategy()->nchans());

    for(unsigned int i=0; i<_plan->dedispersion_strategy()->nchans(); ++i)
    {
        dm_shifts[i] = _plan->dedispersion_strategy()->dmshifts()[i]*_plan->dedispersion_strategy()->dm_step()[_current_dm_range].value()/downsample_factor;
    }

    threaded_dedispersion(_plan->dedispersion_strategy()->kloskis_per_band()[_current_dm_range]
                       , _plan->dedispersion_strategy()->nsamps()/downsample_factor
                       , _plan->dedispersion_strategy()->dedispersed_samples()/downsample_factor
                       , _plan->dedispersion_strategy()->ndms()[_current_dm_range]
                       , dm_shifts
                       , _plan->dedispersion_strategy()->start_dmshifts()[_current_dm_range]
                       , *_plan->dedispersion_strategy()->temp_work_area()
                       , *_plan->dedispersion_strategy()->subanded_dm_trials()
                       );

    for(std::size_t dmidx=0; dmidx<_plan->dedispersion_strategy()->ndms()[_current_dm_range]; ++dmidx)
    integrate_klotski_bruteforce((&*((*_dm_trials_ptr)[dmidx+_total_number_of_dms].begin()))
                     , *_plan->dedispersion_strategy()->subanded_dm_trials()
                     , _plan->dedispersion_strategy()->nchans()
                     , ((*_dm_trials_ptr)[dmidx+_total_number_of_dms].size())
                     , dmidx
                     );

    _total_number_of_dms += _plan->dedispersion_strategy()->ndms()[_current_dm_range];
    ++_current_dm_range;

    return *this;
}

} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
