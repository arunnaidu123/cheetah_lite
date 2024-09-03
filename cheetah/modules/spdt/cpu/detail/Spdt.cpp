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
#include "cheetah/modules/spdt/cpu/detail/MsdEstimator.h"
#include "cheetah/data/SpCandidate.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include "panda/Resource.h"
#include "panda/Log.h"
#include "panda/Error.h"
#include <cstdlib>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace cpu {

namespace detail {

template<class SpdtTraits>
class Spdt<SpdtTraits> : private utils::AlgorithmBase<Config, spdt::Config>
{
    private:
        typedef utils::AlgorithmBase<Config, spdt::Config> BaseT;

    public:
        // mark the architecture this algo is designed for
        typedef cheetah::Cpu Architecture;
        typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;

    private:
        typedef typename SpdtTraits::BufferType BufferType;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;
        typedef std::shared_ptr<typename SpdtTraits::DmTrialsType> SharedDmTrialsType;

    public:
        Spdt(spdt::Config const& config);
        Spdt(Spdt&&) = default;
        Spdt(Spdt const&) = delete;

        /**
         * @brief call the dedispersion/spdt algorithm using the provided device
         */
        std::shared_ptr<typename SpdtTraits::SpType> operator()(panda::PoolResource<cheetah::Cpu>&, SharedDmTrialsType);

        /**
         * @brief performs search on the DM-trial object
         */
        void perform_search(DmTrialsType const& data, std::vector<float>& sp_cands, double mean, double stdev);

    private:
        std::size_t _samples_per_iteration;
        std::size_t _number_of_widths;
        float _threshold;
};

template<class SpdtTraits>
std::shared_ptr<typename SpdtTraits::SpType> Spdt<SpdtTraits>::operator()(panda::PoolResource<panda::Cpu>& cpu
                    , SharedDmTrialsType dm_trials_ptr
                    )
{
    DmTrialsType& dmtrials = *(dm_trials_ptr);
    MsdEstimator<SpdtTraits> msd(dmtrials);
    std::vector<float> spdt_cands;
    perform_search(dmtrials, spdt_cands, msd.mean(), msd.stdev());
    auto sp_candidate_list = std::make_shared<data::SpCcl<uint8_t>>(dm_trials_ptr);

    for (std::size_t idx=0; idx<spdt_cands.size(); idx+=4)
    {
        if(spdt_cands[idx] == 0 && spdt_cands[idx + 1 ]==0 && spdt_cands[idx+2] ==0) break;
        sp_candidate_list->emplace(
                          dmtrials[spdt_cands[idx+2]].dm()
                        , data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType((spdt_cands[idx+1])*dmtrials[spdt_cands[idx+2]].sampling_interval()) // tstart
                        , data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(std::pow(2,spdt_cands[idx+3])*dmtrials[spdt_cands[idx+2]].sampling_interval()) // width
                        , spdt_cands[idx] // sigma
                        );
    }
    return sp_candidate_list;
}

template<typename SpdtTraits>
void Spdt<SpdtTraits>::perform_search(DmTrialsType const& data, std::vector<float>& sp_cands, double mean, double stdev)
{
    std::vector<float> temp(data[0].size());
    for(std::size_t dm_indx=0; dm_indx<data.size(); dm_indx++)
    {
        std::copy(data[dm_indx].begin(), data[dm_indx].end(), temp.begin());
        float downsampling_factor = data.metadata()[dm_indx].downsampling_factor();
        float current_mean = mean;
        unsigned samples_per_iteration = _samples_per_iteration/downsampling_factor;
        unsigned number_of_iterations = data[dm_indx].size()/samples_per_iteration;

        for(unsigned width=0; width<_number_of_widths; width++)
        {
            float current_stdev = (stdev/std::sqrt(std::pow(2,width)))/std::sqrt(downsampling_factor);
            for(unsigned iter=0; iter<number_of_iterations; iter+=samples_per_iteration)
            {
                float t_snr = -1.0;
                float t_sample = 0;
                float t_dm = 0.0;
                float t_width = 0.0;

                for(unsigned i=0; i<samples_per_iteration; i++)
                {
                    unsigned sample = iter*samples_per_iteration+i;
                    float snr = (temp[sample]-current_mean)/current_stdev;
                    if(snr > _threshold && snr > t_snr)
                    {
                        t_snr = snr;
                        t_sample = iter*_samples_per_iteration+i*std::pow(2,width);
                        t_dm = (float)dm_indx;
                        t_width = width;
                        //std::cout<<t_snr<<" "<<t_sample<<" "<<t_dm<<" "<<t_width<<" "<<temp[sample]<<" "<<current_mean<<" "<<current_stdev<<" "<<downsampling_factor<<"\n";
                    }
                }

                if(t_snr>0.0)
                {
                    sp_cands.push_back(t_snr);
                    sp_cands.push_back(t_sample);
                    sp_cands.push_back(t_dm);
                    sp_cands.push_back(t_width);
                }

            }
            for(int sample=0; sample<data[dm_indx].size()/std::pow(2,width+1); ++sample)
            {
                temp[sample] = (temp[2*sample] + temp[2*sample+1])/2.0;
            }
            samples_per_iteration /= 2;

        }
    }
}


template<class SpdtTraits>
Spdt<SpdtTraits>::Spdt(spdt::Config const& config)
    : BaseT(config.cpu_config(), config)
    , _samples_per_iteration(config.cpu_config().samples_per_iteration())
    , _number_of_widths(config.cpu_config().number_of_widths())
    , _threshold(config.threshold())
{
}


} // namespace detail

template<class SpdtTraits>
Spdt<SpdtTraits>::Spdt(spdt::Config const& config)
    : BaseT(config)
{

}

template<class SpdtTraits>
std::shared_ptr<typename SpdtTraits::SpType> Spdt<SpdtTraits>::operator()(panda::PoolResource<Architecture>& dev, SharedDmTrialsType dm_trials_ptr)
{
    return static_cast<BaseT&>(*this)(dev, dm_trials_ptr);
}


} // namespace cpu
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska