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
#ifndef SKA_CHEETAH_MODULES_DDTR_DEDISPERSIONTRIALPLAN_H
#define SKA_CHEETAH_MODULES_DDTR_DEDISPERSIONTRIALPLAN_H


#include "cheetah/utils/Config.h"
#include "cheetah/modules/ddtr/DedispersionConfig.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/data/DmConstant.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Units.h"
#include <vector>
#include <string>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief Configuration module to specify Dedispersion Trials
 * @details
 */

class DedispersionTrialPlan : public utils::Config
{
        typedef utils::Config BaseT;

    public:
        typedef data::DedispersionMeasureType<float> Dm;
        typedef boost::units::quantity<data::dm_constant::s_mhz::Unit, double> DmConstantType;
        typedef boost::units::quantity<boost::units::si::time, double> TimeType;
        typedef boost::units::quantity<data::MegaHertz, double> FrequencyType;
        typedef panda::ConfigModuleIteratorWrapper<DedispersionConfig> RangeIterator;

    public:
        DedispersionTrialPlan(std::string const& tag_name);
        ~DedispersionTrialPlan();

        /**
         * @brief get DM constant
         * @return multiplicative factor giving DM delay in seconds for MHZ frequencies
         */
        DmConstantType dm_constant() const;

        /**
         * @brief set DM constant
         * @return multiplicative factor giving DM delay in seconds for MHZ frequencies
         */
        void dm_constant(DmConstantType dm_const);

        /**
         * @brief get range of DM values
         */
        void add_dm_range(Dm start, Dm end, Dm step);

        /**
         * @brief add a Dedispersion Configuration Element
         * @details note that the lifetime of the passed object is not controlled
         *          by this class.
         */
        void dedispersion_config(DedispersionConfig& config);

        /**
         * @brief Generate metadata based on dedispersion plan
         *
         * @details
         * @throw Error if size of DM overlap region exceeds number of spectra
        */
        std::shared_ptr<data::DmTrialsMetadata> generate_dmtrials_metadata(TimeType sample_interval, std::size_t nspectra, std::size_t nsamples) const;

        /**
         * @brief list of Dm trials and downsample factor pairs
         */
        std::vector<std::pair<Dm, unsigned>> const& dm_trials() const;

        /**
         *  @brief the largest Dm value specified
         */
        Dm max_dm() const;

        /**
         * @brief the maximum delay time for a signal at the maximum Dm in the plan
         */
        TimeType maximum_delay(FrequencyType freq_low, FrequencyType freq_high) const;

        /**
         * @brief the maximum delay offset calculated with the TF blocks properties
         */
        template<typename Arch, typename NumericalRep>
        data::DimensionSize<data::Time> maximum_delay_offset(data::TimeFrequency<Arch, NumericalRep> const& tf) const;

        RangeIterator begin_range() const;
        RangeIterator end_range() const;

        /**
         * @brief maximum number of samples to dedisperse
         * @details algorithm should restrict themselves to dispersing no more than this number
         *          of samples per pass
         */
        std::size_t dedispersion_samples() const;

        /**
         * @brief set maximum number of samples to dedisperso
         * @details algorithsm should configure themselves to adhere to this number as closely as possible.
         *          It is not guaranteed that these number of samples will be processed per pass however.
         */
        void dedispersion_samples(std::size_t);

        /**
         * @brief vector consisting of number of dms per range
         */
        std::vector<std::size_t> const& number_of_dms() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        DmConstantType _dm_constant;
        mutable std::vector<std::pair<Dm, unsigned>> _dm_trials;
        mutable std::vector<std::size_t> _number_of_dms;
        mutable Dm              _max_dm;
        std::size_t             _dedispersion_samples;
};


} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "detail/DedispersionTrialPlan.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_DEDISPERSIONTRIALPLAN_H
