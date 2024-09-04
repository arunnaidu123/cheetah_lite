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
#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DDTR_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DDTR_H

#include "cheetah/Configuration.h"
#ifdef SKA_CHEETAH_ENABLE_NASM
#include "cheetah/modules/ddtr/klotski/detail/DdtrWorker.h"
#include "cheetah/modules/ddtr/klotski/DedispersionPlan.h"
#include "cheetah/modules/ddtr/Config.h"
#include <panda/DeviceLocal.h>
#include <mutex>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

/**
 * @brief A Klotski algorithm to perform DDTR.
 * @details For implementation details please look in the \subpage klotski_developer_guide "developer guide"
 *          More general information on the algorithm can be found in the \subpage klotski_algorithm_guide "algorithm guide".
 */

template<typename DdtrTraits>
class Ddtr
{
    public:
        /** Architecture klotski is based on*/
        typedef cheetah::Cpu Architecture;

        /** klotski's dedispersion plan type */
        typedef klotski::DedispersionPlan<DdtrTraits> DedispersionPlan;

        /** Klotski's config type*/
        typedef klotski::Config Config;

        /**  Numbericalrep of the input TF used for the dedispersion*/
        typedef typename DdtrTraits::value_type NumericalRep;

        /** TimeFrequencyType */
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef typename DdtrTraits::BeamConfigType BeamConfigType;

    private:
        typedef typename DdtrTraits::DedispersionHandler DedispersionHandler;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef ddtr::Config::Dm Dm;
        typedef typename DdtrTraits::BufferFillerType BufferFillerType;
        typedef typename DdtrTraits::BufferType BufferType;
        typedef std::vector<ddtr::Config::Dm> DmListType;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef typename TimeFrequencyType::TimeType TimeType;
        typedef std::vector<FrequencyType> FrequencyListType;

    public:
        Ddtr(BeamConfigType const& beam_config, ddtr::Config const& config);
        Ddtr(Ddtr const&) = delete;
        Ddtr(Ddtr&&);
        ~Ddtr();

        /**
         * @brief dedispersion of time frequency data on CPU
         * @details DmTrialsHandler called when buffer is full.
         * @param data   A TimeFrequency block and cpu resource
         * @tparam call_back A functor called after the dedispersion has been calculated for a specific range of DMs.
         *         The call_back should support the signature void(DMtrialsType const&)
         * @return DmTime sequence i.e. timeseries for each DM trial value
         */
        template <typename CallBackT>
        std::shared_ptr<DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>&
                                                , std::shared_ptr<BufferType> data
                                                , CallBackT const& call_back);

        /**
         * @brief dedispersion of time frequency data on CPU
         * @details DmTrialsHandler called when buffer is full.
         * @param data  A TimeFrequency block
         * @return DmTime sequence i.e. timeseries for each DM trial value
         */
        std::shared_ptr<DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>&, std::shared_ptr<BufferType> data);

        /**
         * @brief sets plan for ddtr
         */
        void plan(DedispersionPlan const& plan);

    private:
        struct DdtrWorkerFactory;

    private:
        std::shared_ptr<DedispersionPlan> _plan;
        DdtrWorker<DdtrTraits> _worker;
};

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/Ddtr.cpp"

#endif // SKA_CHEETAH_ENABLE_NASM
#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DDTR_H
