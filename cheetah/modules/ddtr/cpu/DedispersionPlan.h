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
#ifndef SKA_CHEETAH_MODULES_DDTR_CPU_DEDISPERSIONPLAN_H
#define SKA_CHEETAH_MODULES_DDTR_CPU_DEDISPERSIONPLAN_H

#include "cheetah/modules/ddtr/cpu/DedispersionStrategy.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/modules/ddtr/detail/AggregationBuffer.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/modules/ddtr/cpu/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

/**
 * @brief Dedispersion plan for cpu ddtr
 * @details This object is a necessary for all the DDTR modules which is accessed by CommonDedisperionPlan object to set the plan.
 */
template <typename DdtrTraits>
class DedispersionPlan
{
    private:
        typedef DedispersionStrategy<DdtrTraits> DedispersionStrategyType;

    public:
        typedef typename DdtrTraits::value_type NumericalT;
        typedef typename DdtrTraits::DedispersionHandler DedispersionHandler;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef ddtr::Config::Dm Dm;
        typedef typename DdtrTraits::BufferFillerType BufferFillerType;
        typedef typename DdtrTraits::BufferType BufferType;
        typedef std::vector<ddtr::Config::Dm> DmListType;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef typename TimeFrequencyType::TimeType TimeType;
        typedef std::vector<FrequencyType> FrequencyListType;
        typedef ddtr::Config ConfigType;
        typedef typename DdtrTraits::BeamConfigType BeamConfigType;

    public:
        /**
         * @param memory The maximum memory available on the device (in number of DdtrTraits::value_type values)
         */
        DedispersionPlan(BeamConfigType const&, ConfigType const& config, std::size_t memory);
        ~DedispersionPlan();

        /**
         * @brief reset the plan to be compatible with the TimeFrequency metadata
         * @details note that the number_of_spectra of this TimeFrequencyType object is ignored
         */
        template <typename DataType>
        data::DimensionSize<data::Time> reset(DataType const&);

        /**
         * @brief reset the plan to be compatible with the value
         */
        void reset(data::DimensionSize<data::Time> const& spectra);

        /**
         * @brief return the number of spectra currently configured
         */
        data::DimensionSize<data::Time> number_of_spectra() const;

        /**
         * @brief return the number of spectra in the overlap buffer
         * @details Corresponds to the maximum delay required for the max dm value in the plan
         */
        data::DimensionSize<data::Time> buffer_overlap() const;


        /**
         * @brief return list of Dmtrial and downsampling factor pairs
         */
        std::vector<std::pair<Dm, unsigned>> const& dm_trials() const;

        /**
         * @brief return algo specific dm_factors
         */
        std::vector<double> const& dm_factors() const;

        /**
         * @brief return a DmTrialsMetadata block consistent with the plan and the incoming data parameters
         */
        std::shared_ptr<data::DmTrialsMetadata> dm_trials_metadata(data::TimeFrequencyMetadata const& tf_info, data::DimensionSize<data::Time> number_of_spectra);

        /**
         * @brief return algo_config
         */
        ConfigType const& algo_config() const;

    protected:
        std::shared_ptr<data::DmTrialsMetadata> generate_dmtrials_metadata(TimeType sample_interval, data::DimensionSize<data::Time> nspectra, std::size_t nsamples) const;

    private:
        std::shared_ptr<DedispersionStrategyType> _strategy;
        ConfigType const& _algo_config;
        std::size_t _memory;
        std::shared_ptr<data::DmTrialsMetadata> _dm_trial_metadata;
};

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/cpu/detail/DedispersionPlan.cpp"
#endif // SKA_CHEETAH_MODULES_DDTR_CPU_DEDISPERSIONPLAN_H
