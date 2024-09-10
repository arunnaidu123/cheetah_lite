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
#include "cheetah/modules/spdt/klotski_common/detail/MsdEstimator.h"
#include "cheetah/data/SpCandidate.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "panda/Resource.h"
#include "panda/Log.h"
#include "panda/Error.h"
#include <cstdlib>
#include <algorithm>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace klotski_common {

/**
 * @brief ASM kernel which performs width search on the 8 DM trials at a time.
 * @param stack_variables Array containing all the stack variables.
 * @param total_size Total size of the stack used
 * @param data_in_pointers pointers to the input data elements.
 * @param temp_cands temparay aray containing the all the candidate info.
 * @param widths_array array of widths to be searched
 * @param mst_array Array containg the mean and std values. These are arranged in multiple of 8 doubles one for each DM trial.
 * @param overlap The overlap from previous search
 * @param sum_array Array containing the sum of the samples for each width.
 */

extern "C" void nasm_filter_spdt( std::size_t* stack_variables
                                , std::size_t total_size
                                , std::size_t* data_in_pointers
                                , unsigned int* temp_cands
                                , unsigned int* widths_array
                                , double* mst_array
                                , float* overlap
                                , double* sum_array
                                );

template<class SpdtTraits, typename ImplConfigType, typename AlgoConfigType>
std::shared_ptr<typename SpdtTraits::SpType> KlotskiCommon<SpdtTraits, ImplConfigType, AlgoConfigType>::operator()(panda::PoolResource<panda::Cpu>& cpu, std::shared_ptr<typename SpdtTraits::DmTrialsType> data)
{
    std::vector<float> spdt_cands;
    auto& dmtrials = *data;

    this->perform_search(dmtrials, spdt_cands, dmtrials.metadata().number_of_ranges());

    auto sp_candidate_list = std::make_shared<data::SpCcl<uint8_t>>(data);

    for (std::size_t idx=0; idx<spdt_cands.size(); idx+=4)
    {
        if(spdt_cands[idx] == 0 && spdt_cands[idx + 1 ]==0 && spdt_cands[idx+2] ==0) break;
        sp_candidate_list->emplace(
                          (dmtrials)[spdt_cands[idx+2]].dm()
                        , data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType((spdt_cands[idx+1])*(dmtrials)[spdt_cands[idx+2]].sampling_interval()) // tstart
                        , data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(spdt_cands[idx+3]*(dmtrials)[spdt_cands[idx+2]].sampling_interval()) // width
                        , spdt_cands[idx] // sigma
                        );
    }

    return sp_candidate_list;
}

template<class SpdtTraits, typename ImplConfigType, typename AlgoConfigType>
void KlotskiCommon<SpdtTraits, ImplConfigType, AlgoConfigType>::perform_search(DmTrialsType const& data, std::vector<float>& sp_cands, std::vector<unsigned int> const& ndms)
{

    if(_overlap.size()==0)
    {
        _overlap.resize(ndms.size());
        _sum_array.resize(ndms.size());
        _widths_array_per_range.resize(ndms.size());
    }

    unsigned int start_dm_index = 0;
    for(unsigned int dm_range=0; dm_range<ndms.size(); ++dm_range)
    {
        MsdEstimator<SpdtTraits> msd(data, start_dm_index+(ndms[dm_range]/2));

        unsigned int number_of_dmindex_iterations = ndms[dm_range]/_number_of_dms_per_iteration;
        if(ndms[dm_range]%8!=0) ++number_of_dmindex_iterations; // Making sure that the edge cases are taken care of

        if(_overlap[dm_range].size()==0)
        {
            //initializing overlap and sumarray
            _overlap[dm_range].resize(number_of_dmindex_iterations);
            for(unsigned int i=0; i<number_of_dmindex_iterations; ++i)
            {
                _overlap[dm_range][i].resize(_max_width*_number_of_dms_per_iteration/std::pow(2, dm_range));
                std::fill(_overlap[dm_range][i].begin(), _overlap[dm_range][i].end(), 0.0f);
            }

            std::vector<unsigned int> temp_vector;
            for(unsigned int i=0; i<_widths_array.size(); ++i)
            {
                if(_widths_array[i]<_max_width/std::pow(2, dm_range)) _widths_array_per_range[dm_range].push_back(_widths_array[i]);
            }

            _sum_array[dm_range].resize(number_of_dmindex_iterations);
            for(unsigned int i=0; i<number_of_dmindex_iterations; ++i)
            {
                _sum_array[dm_range][i].resize(_number_of_dms_per_iteration*_widths_array_per_range[dm_range].size());
                std::fill(_sum_array[dm_range][i].begin(), _sum_array[dm_range][i].end(), 0.0);
            }
        }

        if(_temp_cands.size()==0) _temp_cands.resize(4*data[0].size()*_widths_array.size());

        for(std::size_t dm_indx=0; dm_indx<ndms[dm_range]; dm_indx+=_number_of_dms_per_iteration)
        {
            if(dm_indx+_number_of_dms_per_iteration>ndms[dm_range]) dm_indx = ndms[dm_range]-_number_of_dms_per_iteration;

            float current_mean = msd.mean();
            float current_stdev = msd.stdev();
            unsigned int number_of_candidates = 0;
            call_nasm_filter_spdt(data
                    , dm_indx+start_dm_index
                    , current_mean
                    , current_stdev
                    , number_of_candidates
                    , dm_range
                    , dm_indx/_number_of_dms_per_iteration);

            unsigned int count = 0;


            while(count<number_of_candidates)
            {
                unsigned int window_size = _temp_cands[count*4+2]/2;
                if(window_size==0)  window_size = 1;

                unsigned int current_start_sample = std::floor(_temp_cands[count*4+3]/window_size)*window_size;
                unsigned int best_snr = 0;
                unsigned int best_sample = 0;
                unsigned int best_dm = 0;
                unsigned int best_width = 0;

                while(count<number_of_candidates && _temp_cands[count*4+3]<current_start_sample+window_size)
                {
                    if(best_snr<_temp_cands[count*4])
                    {
                        best_snr=_temp_cands[count*4];
                        best_sample = _temp_cands[count*4+3];
                        best_dm = _temp_cands[count*4+1];
                        best_width = _temp_cands[count*4+2];
                    }
                    ++count;
                }

                if(best_snr!=0)
                {
                    sp_cands.push_back(((float)best_snr/100.0)); //snr
                    sp_cands.push_back((float)best_sample-(float)(best_width/2)); // sample (sub tract half the width to center the candidate)
                    sp_cands.push_back((float)(best_dm)); // dm
                    sp_cands.push_back((float)(best_width)); // width
                }
            }
        }
        start_dm_index += ndms[dm_range];
    }
}

template<class SpdtTraits, typename ImplConfigType, typename AlgoConfigType>
void KlotskiCommon<SpdtTraits, ImplConfigType, AlgoConfigType>::call_nasm_filter_spdt(DmTrialsType const& data_in
                , unsigned int start_dmindx
                , float mean
                , float stdev
                , unsigned int& number_of_candidates
                , unsigned int dm_range
                , unsigned int iteration)
{
    std::vector<std::size_t> stack_variables(23);

    stack_variables[0] = 64; //DATA_IN_POINTERS_SIZE
    stack_variables[1] = ((_max_width/std::pow(2,dm_range))+8)*_number_of_dms_per_iteration*sizeof(float); //SCRATCH_SIZE
    stack_variables[2] = 64*sizeof(float); //TEMP_SIZE
    stack_variables[3] = 64*sizeof(float); //TEMP_CORNERTURNED_SIZE
    stack_variables[4] = 64*_widths_array_per_range[dm_range].size(); // SUM_ARRAY_SIZE
    stack_variables[5] = _widths_array_per_range[dm_range].size()*sizeof(int); // WIDTHS_ARRAY_SIZE
    stack_variables[6] = 64; // MEAN_ARRAY_SIZE
    stack_variables[7] = 64*_widths_array_per_range[dm_range].size(); // STD_ARRAY_SIZE
    stack_variables[8] = 64; // THRESHOLD_ARRAY_SIZE
    stack_variables[9] = 256; //DATA_IN_POINTERS_LOCATION
    stack_variables[10] = stack_variables[9]+stack_variables[0]; //SCRATCH_LOCATION
    stack_variables[11] = stack_variables[10]+stack_variables[1]; //TEMP_LOCATION
    stack_variables[12] = stack_variables[11]+stack_variables[2]; //TEMP_CORNERTURNED_LOCATION
    stack_variables[13] = stack_variables[12]+stack_variables[3]; //SUM_ARRAY_LOCATION
    stack_variables[14] = stack_variables[13]+stack_variables[4]; // WIDTHS_ARRAY_LOCATION
    stack_variables[15] = stack_variables[14]+stack_variables[5]; // MEAN_ARRAY_LOCATION
    stack_variables[16] = stack_variables[15]+stack_variables[6]; // STD_ARRAY_LOCATION
    stack_variables[17] = stack_variables[16]+stack_variables[7]; // THRESHOLD_ARRAY_LOCATION
    stack_variables[18] = _widths_array_per_range[dm_range].size(); // NUMBER_OF_WIDTHS
    stack_variables[19] = (_max_width/std::pow(2,dm_range)); // MAX_WIDTH
    stack_variables[20] = start_dmindx; // START_DMINDX
    stack_variables[21] = data_in[start_dmindx].size()*sizeof(float); // SAMPLE_SIZE_PER_DMINDX
    stack_variables[22] = stack_variables[17]+stack_variables[8]; //TOTAL_SIZE

    std::vector<const std::size_t*> data_in_pointers(_number_of_dms_per_iteration);
    for(std::size_t i=0; i<_number_of_dms_per_iteration; ++i)
    {
        data_in_pointers[i]=reinterpret_cast<const std::size_t*>(&*(data_in[start_dmindx+i].begin()));
    }

    std::vector<double> mst_array((2+_widths_array_per_range[dm_range].size())*8);

    for(unsigned int t=0; t<8; ++t)
    {
        mst_array[t] = mean;
        mst_array[t+8] = _threshold;
    }

    for(unsigned int i=0; i<_widths_array_per_range[dm_range].size(); ++i)
    {
        for(unsigned int t=0; t<8; ++t)
        mst_array[16+i*8+t] = stdev/std::sqrt(_widths_array_per_range[dm_range][i]);
    }



    nasm_filter_spdt( &*stack_variables.begin()
                    , stack_variables[22]
                    , reinterpret_cast<std::size_t*>(&*data_in_pointers.begin())
                    , &*_temp_cands.begin()
                    , &*_widths_array_per_range[dm_range].begin()
                    , &*mst_array.begin()
                    , &*_overlap[dm_range][iteration].begin()
                    , &*_sum_array[dm_range][iteration].begin()
                    );
    number_of_candidates = stack_variables[0];
}

template<class SpdtTraits, typename ImplConfigType, typename AlgoConfigType>
KlotskiCommon<SpdtTraits, ImplConfigType, AlgoConfigType>::KlotskiCommon(ImplConfigType const& klotski_config, AlgoConfigType const& config)
    : BaseT(klotski_config, config)
    , _threshold(config.threshold())
    , _max_width(klotski_config.max_width_limit())
    , _temp_cands(0)
    , _widths_array(klotski_config.widths())
    , _widths_array_per_range(0)
    , _overlap(0)
    , _sum_array(0)
    , _number_of_dms_per_iteration(8) //TODO: This value can be 8, 16, 32. However, we fix this 8 asa higher value needs more cache to implement.
{
}

} // namespace klotski_common
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska