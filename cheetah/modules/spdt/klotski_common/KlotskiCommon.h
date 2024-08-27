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
#ifndef SKA_CHEETAH_MODULES_SPDT_KLOTSKI_COMMON_KLOTSKICOMMON_H
#define SKA_CHEETAH_MODULES_SPDT_KLOTSKI_COMMON_KLOTSKICOMMON_H

#ifdef SKA_CHEETAH_ENABLE_NASM

#include "cheetah/modules/ddtr/klotski/Ddtr.h"
#include "cheetah/modules/spdt/detail/CommonTypes.h"
#include "cheetah/modules/spdt/Config.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/utils/Architectures.h"
#include "cheetah/utils/AlgorithmBase.h"
#include <mutex>
#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace klotski_common {

/**
 * @brief Single pulse search on AVX
 * @details This not a asynchronous version
 */

template<class SpdtTraits, typename DdtrType, typename ImplConfigType, typename AlgoConfigType>
class KlotskiCommon : private utils::AlgorithmBase<ImplConfigType, AlgoConfigType>
{
    private:
        typedef utils::AlgorithmBase<ImplConfigType, AlgoConfigType> BaseT;

    public:
        // mark the architecture this algo is designed for
        typedef cheetah::Cpu Architecture;
        typedef typename DdtrType::DedispersionPlan DedispersionPlan;

    public:
        typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;

    private:
        typedef typename SpdtTraits::BufferType BufferType;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;

    public:
        KlotskiCommon(ImplConfigType const& klotski_config, AlgoConfigType const& config);
        KlotskiCommon(KlotskiCommon&&) = default;
        KlotskiCommon(KlotskiCommon const&) = delete;

        /**
         * @brief call the dedispersion/spdt algorithm using the provided device
         */
        template<typename SpHandler>
        std::shared_ptr<typename SpdtTraits::DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>&, BufferType&, SpHandler const&);

        /**
         * @brief set the dedispersion plan
         */
        void plan(DedispersionPlan const& plan);

        /**
         * @brief performs search on the DM-trial object
         */
        void perform_search(DmTrialsType const& data, std::vector<float>& sp_cands, std::vector<unsigned int> const& ndms);

        /**
        * @brief the function which call ASM kernel that performs bached width search for 8 DMs at a time.
        * The output candidates are written in the _temp_cands.
        * @param data_in DMTrials data
        * @param start_dmindx the start dm of the batch of 8
        * @param mean the estimated mean of the DMTrails data
        * @param stdev standard deviation of data.
        * @param number_of_candidates number of candidates in this batch
        * @param dm_range the current dm range
        * @param iteration the current iteration value
        */
        void call_nasm_filter_spdt(DmTrialsType const& data_in
                , unsigned int start_dmindx
                , float mean
                , float stdev
                , unsigned int& number_of_candidates
                , unsigned int dm_range
                , unsigned int iteration);

    private:
        DdtrType _ddtr;
        std::size_t _samples_per_iteration;
        float _threshold;
        std::size_t _max_width;
        std::vector<unsigned int> _temp_cands;
        std::vector<unsigned int> _widths_array;
        std::vector<std::vector<unsigned int>> _widths_array_per_range;
        std::vector<std::vector<std::vector<float>>> _overlap;
        std::vector<std::vector<std::vector<double>>> _sum_array;
        unsigned int _number_of_dms_per_iteration;
};


} // namespace klotski_common
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/spdt/klotski_common/detail/KlotskiCommon.cpp"

#endif // SKA_CHEETAH_ENABLE_NASM
#endif // SKA_CHEETAH_MODULES_SPDT_KLOTSKI_COMMON_KLOTSKICOMMON_H