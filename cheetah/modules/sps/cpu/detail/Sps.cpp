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
#include "cheetah/modules/sps/cpu/detail/MsdEstimator.h"
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
namespace sps {
namespace cpu {

namespace detail {

template<class SpsTraits>
class Sps<SpsTraits> : private utils::AlgorithmBase<Config, sps::Config>
{
    private:
        typedef utils::AlgorithmBase<Config, sps::Config> BaseT;

    public:
        // mark the architecture this algo is designed for
        typedef cheetah::Cpu Architecture;
        typedef ddtr::cpu::DedispersionPlan<SpsTraits> DedispersionPlan;

    public:
        typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;

    private:
        typedef typename SpsTraits::BufferType BufferType;
        typedef typename SpsTraits::DmTrialsType DmTrialsType;

    public:
        Sps(sps::Config const& config);
        Sps(Sps&&) = default;
        Sps(Sps const&) = delete;

        /**
         * @brief call the dedispersion/sps algorithm using the provided device
         */
        template<typename SpHandler>
        std::shared_ptr<typename SpsTraits::DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>&, BufferType&, SpHandler const&);

        /**
         * @brief set the dedispersion plan
         */
        void plan(DedispersionPlan const& plan);

        /**
         * @brief performs search on the DM-trial object
         */
        void perform_search(DmTrialsType const& data, std::vector<float>& sp_cands, double mean, double stdev);

    private:
        ddtr::cpu::Ddtr<SpsTraits> _ddtr;
        std::size_t _samples_per_iteration;
        std::size_t _number_of_widths;
        float _threshold;
};

template<class SpsTraits>
template<typename SpHandler>
std::shared_ptr<typename SpsTraits::DmTrialsType> Sps<SpsTraits>::operator()(panda::PoolResource<panda::Cpu>& cpu
                    , BufferType& agg_buf
                    , SpHandler const& sp
                    )
{

    std::shared_ptr<DmTrialsType> dm_trials_ptr = _ddtr(cpu, agg_buf);
    DmTrialsType& dmtrials = *(dm_trials_ptr);
    MsdEstimator<SpsTraits> msd(dmtrials);
    std::vector<float> sps_cands;
    perform_search(dmtrials, sps_cands, msd.mean(), msd.stdev());
    auto sp_candidate_list = std::make_shared<data::SpCcl<uint8_t>>(agg_buf.composition()
                , data::DimensionIndex<data::Time>(agg_buf.offset_first_block()/agg_buf.composition()[0]->number_of_channels()));
    sp_candidate_list->reserve(sps_cands.size()/4);

    for (std::size_t idx=0; idx<sps_cands.size(); idx+=4)
    {
        if(sps_cands[idx] == 0 && sps_cands[idx + 1 ]==0 && sps_cands[idx+2] ==0) break;
        sp_candidate_list->emplace(
                          dmtrials[sps_cands[idx+2]].dm()
                        , data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType((sps_cands[idx+1])*dmtrials[sps_cands[idx+2]].sampling_interval()) // tstart
                        , data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(std::pow(2,sps_cands[idx+3])*dmtrials[sps_cands[idx+2]].sampling_interval()) // width
                        , sps_cands[idx] // sigma
                        );
    }

    sp(sp_candidate_list);
    return dm_trials_ptr;
}

template<typename SpsTraits>
void Sps<SpsTraits>::perform_search(DmTrialsType const& data, std::vector<float>& sp_cands, double mean, double stdev)
{
    std::vector<float> temp(data[0].size());
    for(std::size_t dm_indx=0; dm_indx<data.size(); dm_indx++)
    {
        std::copy(data[dm_indx].begin(), data[dm_indx].end(), temp.begin());
        float downsampling_factor = data[dm_indx].sampling_interval()/data.metadata().fundamental_sampling_interval();
        float current_mean = mean;
        float current_stdev = stdev/std::sqrt(downsampling_factor);
        unsigned samples_per_iteration = _samples_per_iteration/downsampling_factor;
        unsigned number_of_iterations = data[dm_indx].size()/samples_per_iteration;

        for(unsigned width=0; width<_number_of_widths; width++)
        {
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
            current_stdev = (stdev/std::sqrt(std::pow(2,width+1)))/std::sqrt(downsampling_factor);
        }
    }
}


template<class SpsTraits>
Sps<SpsTraits>::Sps(sps::Config const& config)
    : BaseT(config.cpu_config(), config)
    , _ddtr(config.ddtr_config())
    , _samples_per_iteration(config.cpu_config().samples_per_iteration())
    , _number_of_widths(config.cpu_config().number_of_widths())
    , _threshold(config.threshold())
{
}

template<class SpsTraits>
void Sps<SpsTraits>::plan(DedispersionPlan const& plan)
{
    _ddtr.plan(plan);
}

} // namespace detail

template<class SpsTraits>
Sps<SpsTraits>::Sps(sps::Config const& config)
    : BaseT(config)
{

}

template<class SpsTraits>
template<typename SpHandler, typename BufferType>
std::shared_ptr<typename SpsTraits::DmTrialsType> Sps<SpsTraits>::operator()(panda::PoolResource<Architecture>& dev, BufferType& buf, SpHandler& sh)
{
    return static_cast<BaseT&>(*this)(dev, buf, sh);
}


} // namespace cpu
} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska