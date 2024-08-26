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
#ifndef SKA_CHEETAH_MODULES_DDTR_CPU_DEDISPERSIONSTRATEGY_H
#define SKA_CHEETAH_MODULES_DDTR_CPU_DEDISPERSIONSTRATEGY_H

#include "cheetah/data/TimeFrequency.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/modules/ddtr/cpu/Config.h"
#include <cstddef>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

/**
 * @brief Dedispersion strategy object for cpu
 * @details This object essentially contains all the overhead and information needed to perform DDTR.
 */
template <typename DdtrTraits>
class DedispersionStrategy
{
    public:
        typedef typename DdtrTraits::value_type NumericalT;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef ddtr::Config::Dm Dm;
        typedef typename DdtrTraits::BufferFillerType BufferFillerType;
        typedef typename DdtrTraits::BufferType BufferType;
        typedef std::vector<ddtr::Config::Dm> DmListType;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef typename TimeFrequencyType::TimeType TimeType;
        typedef std::vector<FrequencyType> FrequencyListType;

    public:
        DedispersionStrategy(TimeFrequencyType const& chunk,  const ddtr::Config& config, int const& memory);
        ~DedispersionStrategy();

        /**
         * @brief reset the plan to be compatible with the TimeFrequency metadata
         * @details note that the number_of_spectra of this TimeFrequencyType object is ignored
         */
        data::DimensionSize<data::Time> make_strategy(TimeFrequencyType const& chunk,  const ddtr::Config& config);

        /**
         * @brief return the number of spectra currently configured
         */
        data::DimensionSize<data::Time> number_of_spectra() const;

        /**
         * @brief set the number of spectra currently configured
         */
        void number_of_spectra(data::DimensionSize<data::Time> const& spectra);

        /**
         * @brief return the number of spectra in the overlap buffer.
         * @details Corresponds to the maximum delay required for the max dm value in the plan.
         */
        data::DimensionSize<data::Time> buffer_overlap() const;

        /**
         * @brief return algo specific dm_trials.
         */
        std::vector<Dm> const& dm_trials() const;

        /**
         * @brief return algo specific dm_factors.
         */
        std::vector<double> const& dm_factors() const;

        /**
         * @brief return user defined number of dm ranges.
         */
        std::size_t const& number_of_dm_ranges() const;

    private:
        std::size_t _memory;
        data::DimensionSize<data::Time> _max_delay;
        data::DimensionSize<data::Time> _number_of_spectra;
        std::vector<double> _dm_factors;
        std::vector<Dm> _user_dm_low;
        std::vector<Dm> _user_dm_high;
        std::vector<Dm> _user_dm_step;
        std::size_t _number_of_dm_ranges;
};

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/cpu/detail/DedispersionStrategy.cpp"
#endif // SKA_CHEETAH_MODULES_DDTR_CPU_DEDISPERSIONSTRATEGY_H