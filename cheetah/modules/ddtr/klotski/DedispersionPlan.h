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
#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DEDISPERSIONPLAN_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DEDISPERSIONPLAN_H

#include "cheetah/modules/ddtr/klotski/Config.h"
#include "cheetah/modules/ddtr/klotski/detail/DedispersionStrategy.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/utils/MultiThread.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

/**
 * @brief Dedipsersion plan for the klotski module
 */

template <typename DdtrTraits>
class DedispersionPlan
{
    private:
        typedef typename DdtrTraits::value_type NumericalT;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef DedispersionStrategy<NumericalT> DedispersionStrategyType;
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef ddtr::Config::Dm Dm;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef typename TimeFrequencyType::TimeType TimeType;
        typedef std::vector<FrequencyType> FrequencyListType;
        typedef ddtr::Config ConfigType;
        typedef typename DdtrTraits::BeamConfigType BeamConfigType;

    public:
        DedispersionPlan(BeamConfigType const& beam_config, ConfigType const& config, std::size_t memory=0);

        /**
         * @brief takes in TF chunk and gernerates a strategy object returning the
         *        number of spectra.
         */
        data::DimensionSize<data::Time> reset(TimeFrequencyType const&);

        /**
         * @brief sets the number of spectra private member
         */
        void reset(data::DimensionSize<data::Time> const& spectra);

        /**
         * @brief returns number of spectra
         */
        data::DimensionSize<data::Time> number_of_spectra() const;

        /**
         * @brief essentially returns maxshift
         */
        data::DimensionSize<data::Time> buffer_overlap() const;

        /**
         * @brief generates dmtrials metadata needed for generating dmtrials object
         */
        std::shared_ptr<data::DmTrialsMetadata> generate_dmtrials_metadata(TimeType sample_interval, std::size_t nspectra) const;

        /**
         * @brief returns dmtrials metadata used for generating dmtrials object
         */
        std::shared_ptr<data::DmTrialsMetadata> dm_trial_metadata() const;

        /**
         * @brief returns pointer to the DedispersionStrategy
         */
        std::shared_ptr<DedispersionStrategyType> const& dedispersion_strategy() const;

        /**
         * @brief returns pointer to the DmTrials
         */
        std::shared_ptr<DmTrialsType> const& dm_trials();

        std::shared_ptr<DmTrialsType> const& spdt_dm_trials();

        std::vector<unsigned> const& affinities();

        unsigned current_dm_range() { return _current_dm_range;}

        void current_dm_range(unsigned val)
        {
            _current_dm_range = val;
        }

        static void call_serial_dedispersion(std::shared_ptr<DedispersionPlan> plan, unsigned start_channel, unsigned band);

        void initialize_threads();

        std::string const& beam_id()
        {
            return _beam_config.id();
        }

    private:
        BeamConfigType const& _beam_config;
        ConfigType const& _config;
        std::shared_ptr<DedispersionStrategyType> _strategy;
        std::shared_ptr<data::DmTrialsMetadata> _dm_trial_metadata;
        std::size_t _memory;
        std::size_t _max_delay;
        std::size_t _dedispersion_samples;
        std::vector<double> _dm_factors;
        std::size_t _number_of_spectra;
        std::shared_ptr<DmTrialsType> _dm_trials_ptr;
        std::shared_ptr<DmTrialsType> _spdt_dm_trials_ptr;
        unsigned _current_dm_range;
        //utils::MultiThread _ddtr_threads;
};

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/klotski/detail/DedispersionPlan.cpp"
#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DEDISPERSIONPLAN_H
