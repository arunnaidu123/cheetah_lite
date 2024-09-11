/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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

#include "cheetah/modules/ddtr/klotski/DdtrProcessor.h"
#include "cheetah/data/FrequencyTime.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

extern "C" void nasm_integrate(std::size_t *data_out_pointers
                                , std::size_t *data_sub_pointers
                                , const std::size_t *dmshift_pointers
                                , const unsigned int *dsamps_per_band
                                , std::size_t *stack_variables
                                , std::size_t total_size_of_stack
                                );

extern "C" void nasm_downsample(unsigned short *data, unsigned int number_of_elements);

extern "C" void nasm_zeros(int *data, std::size_t bytes);

int serial_dedispersion(std::vector<int>& data_out
                           , std::vector<unsigned short>& data_in
                           , std::vector<unsigned int> dsamps_per_klotski
                           , int nsamps
                           , int number_of_dms
                           , int max_channels_per_klotski
                           , int nchans
                           , std::vector<std::vector<unsigned int>> dmindex_shifts
                           , std::vector<std::vector<unsigned int>> total_base
                           , std::vector<std::vector<unsigned int>> total_index
                           , std::vector<std::vector<unsigned int>> total_shift
                           , std::vector<std::vector<unsigned int>> counts_array
                           , std::vector<unsigned int> const& start_dm_shifts
                           , unsigned int channels_offset
                           );

template<typename DdtrTraits>
DdtrProcessor<DdtrTraits>::DdtrProcessor(std::shared_ptr<DedispersionPlanType> plan
                                        , std::shared_ptr<DmTrialsType> dm_trials_ptr
                            )
    : _plan(plan)
    , _dm_trials_ptr(dm_trials_ptr)
    , _current_dm_range(0)
    , _total_number_of_dms(0)
    , _start_dm_value(0)
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
    threaded_dedispersion(_plan);

    nasm_downsample(&*(*_plan->dedispersion_strategy()->temp_work_area()).begin()
                   , _plan->dedispersion_strategy()->nsamps()*_plan->dedispersion_strategy()->nchans());

    ++_current_dm_range;

    return *this;
}

template<typename DdtrTraits>
void DdtrProcessor<DdtrTraits>::integrate_reference( DmTrialsType& data_out
                                                    , std::vector<std::vector<int>>& data_temp
                                                    , std::vector<std::vector<unsigned int>>& dmshift_per_band
                                                    , std::vector<std::vector<unsigned int>> const& dsamps_per_klotski
                                                    , unsigned int number_of_channels
                                                    , unsigned int ndms
                                                    , unsigned int start_dm_value)
{

    unsigned int number_of_bands = dsamps_per_klotski.size();

    std::vector<std::size_t> data_out_pointers;
    for(std::size_t i=start_dm_value; i<start_dm_value+ndms; ++i)
    {
        data_out_pointers.emplace_back(reinterpret_cast<std::size_t>(&*data_out[i].begin()));
    }

    std::vector<std::size_t*> data_sub_pointers;
    for(std::size_t i=0; i<number_of_bands; ++i)
    {
        data_sub_pointers.push_back(reinterpret_cast<std::size_t*>(&*data_temp[i].begin()));
    }


    std::vector<std::size_t*> dmshift_pointers;
    for(std::size_t i=0; i<number_of_bands; ++i)
    {
        dmshift_pointers.push_back(reinterpret_cast<std::size_t*>(&*dmshift_per_band[i].begin()));
    }

    std::vector<unsigned int> dsamps_per_band(number_of_bands);

    for(unsigned int band=0; band<data_temp.size(); ++band)
    {
        dsamps_per_band[band] = dsamps_per_klotski[band][0];
    }

    std::vector<std::size_t> stack_variables(9);

    stack_variables[0] = 128; //DATA_OUT_POINTERS
    stack_variables[1] = stack_variables[0]+ndms*sizeof(std::size_t); //DATA_SUB_POINTERS
    stack_variables[2] = stack_variables[1]+number_of_bands*sizeof(std::size_t); //DMSHIFTS_LOCATION
    stack_variables[3] = stack_variables[2]+ ndms*number_of_bands*sizeof(std::size_t); //DSAMPS_PER_BAND_LOCATION
    stack_variables[4] = number_of_channels; //NCHANS
    stack_variables[5] = number_of_bands; //NBANDS
    stack_variables[6] = ndms; //NDMS
    stack_variables[7] = dsamps_per_klotski[0][0]/16; //NITER
    stack_variables[8] = stack_variables[3]+number_of_bands*sizeof(unsigned int); //STACK_SIZE


    nasm_integrate( reinterpret_cast<std::size_t*>(&*data_out_pointers.begin())
                  , reinterpret_cast<std::size_t*>(&*data_sub_pointers.begin())
                  , reinterpret_cast<std::size_t*>(&*dmshift_pointers.begin())
                  , reinterpret_cast<const unsigned int*>(&*dsamps_per_band.begin())
                  , &*stack_variables.begin()
                  , stack_variables[8]
                  );

}

template<typename DdtrTraits>
void DdtrProcessor<DdtrTraits>::call_serial_dedispersion(std::shared_ptr<DedispersionPlanType> plan, unsigned start_channel, unsigned band)
{
    serial_dedispersion( std::ref((*plan->dedispersion_strategy()->subanded_dm_trials())[band])
                             , std::ref(*plan->dedispersion_strategy()->temp_work_area())
                             , plan->dedispersion_strategy()->dsamps_per_klotski()[plan->current_dm_range()][band]
                             , plan->dedispersion_strategy()->nsamps()/std::pow(2,plan->current_dm_range())
                             , plan->dedispersion_strategy()->ndms()[plan->current_dm_range()]
                             , plan->dedispersion_strategy()->max_channels_per_klotski()
                             , plan->dedispersion_strategy()->channels_per_band()[band]
                             , plan->dedispersion_strategy()->dmshifts_per_klotski()[plan->current_dm_range()][band]
                             , plan->dedispersion_strategy()->total_base()[plan->current_dm_range()][band]
                             , plan->dedispersion_strategy()->total_index()[plan->current_dm_range()][band]
                             , plan->dedispersion_strategy()->total_shift()[plan->current_dm_range()][band]
                             , plan->dedispersion_strategy()->counts_array()[plan->current_dm_range()][band]
                             , plan->dedispersion_strategy()->start_dm_shifts()[plan->current_dm_range()]
                             , start_channel
                             );
}



template<typename DdtrTraits>
void DdtrProcessor<DdtrTraits>::threaded_dedispersion(std::shared_ptr<DedispersionPlanType> plan)
{
    auto& data_temp = *plan->dedispersion_strategy()->subanded_dm_trials();
    for(unsigned int value=0; value<data_temp.size(); ++value)
    {
        std::memset(&*data_temp[value].begin(), 0, data_temp[value].size()*sizeof(int));
        //std::fill(data_temp[value].begin(), data_temp[value].end(), 0);
        //nasm_zeros(&*data_temp[value].begin(), data_temp[value].size()*sizeof(int));
    }

    _plan->current_dm_range(_current_dm_range);

    if(plan->dedispersion_strategy()->ddtr_threads().number_of_jobs()==0)
    {
        unsigned start_channel = 0;
        for(unsigned int band=0; band<plan->dedispersion_strategy()->number_of_bands(); ++band)
        {
            plan->dedispersion_strategy()->ddtr_threads().add_job(plan->affinities()[band+2]
                                                            , call_serial_dedispersion
                                                            , plan
                                                            , start_channel
                                                            , band
                                                            );
            start_channel += plan->dedispersion_strategy()->channels_per_band()[band];
        }

    }

    for(unsigned int band=0; band<plan->dedispersion_strategy()->number_of_bands(); ++band)
    {
        plan->dedispersion_strategy()->ddtr_threads().ready(band);
    }


    for(unsigned int band=0; band<plan->dedispersion_strategy()->number_of_bands(); ++band)
    {
        plan->dedispersion_strategy()->ddtr_threads().finish(band);
    }


    DmTrialsType& dmtrials = *(_dm_trials_ptr);

    integrate_reference(dmtrials
                    , data_temp
                    , (plan->dedispersion_strategy()->dmshifts_per_band()[_current_dm_range])
                    , plan->dedispersion_strategy()->dsamps_per_klotski()[_current_dm_range]
                    , plan->dedispersion_strategy()->nchans()
                    , plan->dedispersion_strategy()->ndms()[_current_dm_range]
                    , _start_dm_value
                    );

    _start_dm_value += plan->dedispersion_strategy()->ndms()[_current_dm_range];
}

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska