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
#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DDTRPROCESSOR_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DDTRPROCESSOR_H

#include "cheetah/Configuration.h"
#ifdef SKA_CHEETAH_ENABLE_NASM

#include "cheetah/modules/ddtr/klotski/DedispersionPlan.h"
#include "cheetah/data/cuda/FrequencyTime.h"
#include "cheetah/data/DmTrials.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

/**
 * @brief Ddtr processor object with ++ operator which essentially calls the NASM DDTR kernels
 * @details This object takes in FT object and fills up dmtrials object
 */

template<typename DdtrTraits>
class DdtrProcessor
{
        typedef typename DdtrTraits::value_type NumericalRep;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef DedispersionPlan<DdtrTraits> DedispersionPlanType;

    public:
        /**
         * @brief Contructor for DdtrProcessor
         * @param plan DedispersionPlan object
         * @param dmt DMtrails object resultant DM-Time data
         */
        DdtrProcessor( std::shared_ptr<DedispersionPlanType> plan
                     , std::shared_ptr<DmTrialsType> dmt
                     );

        /**
         * @brief process the next step of the DedispersionPlan
         */
        DdtrProcessor& operator++();

        /**
         * @return true if there is no more processing to do
         */
        bool finished() const;

        /**
         * @return DedispersionPlan object
         *
         */
        std::shared_ptr<DedispersionPlanType> dedispersion_plan();

        /**
         * @return current_dm_range
         *
         */
        std::size_t const& current_dm_range() const;

        /**
         * @brief function to multi-thread dedispersion function.
         *
         */
        void threaded_dedispersion(std::shared_ptr<DedispersionPlanType> plan);

        /**
         * @brief Integrate all the subbanded Dmtrials and store it in final DmTrials.
         *
         */
        void integrate_reference( DmTrialsType& data_out
                                , std::vector<std::vector<int>>& data_temp
                                , std::vector<std::vector<unsigned int>>& dmshift_per_band
                                , std::vector<std::vector<unsigned int>> const& dsamps_per_klotski
                                , unsigned int number_of_channels
                                , unsigned int ndms
                                , unsigned int start_dm_value
                                );

        /**
         * @brief calls serial dedispersion kernel to perform dedispersion with in a band.
         */
        static void call_serial_dedispersion(std::shared_ptr<DedispersionPlanType> plan, unsigned start_channel, unsigned band);

    private:
        std::shared_ptr<DedispersionPlanType> _plan;
        std::shared_ptr<DmTrialsType> _dm_trials_ptr;
        std::size_t _current_dm_range;
        std::size_t _total_number_of_dms;
        unsigned int _start_dm_value;
};


} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/DdtrProcessor.cpp"

#endif //SKA_CHEETAH_ENABLE_NASM
#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DDTRPROCESSOR_H
