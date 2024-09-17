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
#ifndef SKA_CHEETAH_MODULES_DDTR_CPU_DDTRPROCESSOR_H
#define SKA_CHEETAH_MODULES_DDTR_CPU_DDTRPROCESSOR_H

#include "cheetah/modules/ddtr/cpu/DedispersionPlan.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/data/DmTrials.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

/**
 * @brief Processing object where each dm_range is dedispersed
 * @details This object must have a ++ operator to process the FT block.
 * This fills the Dm_trials object with the dedispersed data.
 */

template<typename DdtrTraits>
class DdtrProcessor
{
        typedef typename DdtrTraits::BufferType BufferType;
        typedef typename DdtrTraits::value_type NumericalRep;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef typename data::FrequencyTime<Cpu, NumericalRep> FrequencyTimeType;
        typedef DedispersionPlan<DdtrTraits> DedispersionPlanType;

    public:
        DdtrProcessor( std::shared_ptr<DedispersionPlanType>
                     , BufferType&
                     , std::shared_ptr<DmTrialsType> dm_trials_ptr
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
         * @return current_dm_range
         *
         */
        std::size_t const& current_dm_range() const;

        /**
         * @return processed_samples
         *
         */
        data::DimensionIndex<data::Time> const& processed_samples() const;

    private:
        /**
         * @details Downsample the TimeFrequency data by a factor of 2
         *
         */
        void downsample_data();

    private:
        std::shared_ptr<DedispersionPlanType>  _plan;
        BufferType& _ft_data;
        std::shared_ptr<DmTrialsType> _dm_trials_ptr;
        std::size_t _current_dm_range;
        std::size_t _current_dm_idx;
};


} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/DdtrProcessor.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_CPU_DDTRPROCESSOR_H
