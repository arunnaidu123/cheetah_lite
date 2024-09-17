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
#ifndef SKA_CHEETAH_MODULES_DDTR_CPU_DDTR_H
#define SKA_CHEETAH_MODULES_DDTR_CPU_DDTR_H

#include "cheetah/modules/ddtr/cpu/Config.h"
#include "cheetah/modules/ddtr/cpu/DedispersionPlan.h"
#include "cheetah/modules/ddtr/cpu/detail/Worker.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

template<typename DdtrTraits>
class Ddtr
{
    private:
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef typename DdtrTraits::BufferType BufferType;

    public:
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef cpu::DedispersionPlan<DdtrTraits> DedispersionPlan;
        typedef cpu::Config Config;
        typedef Cpu Architecture;
        typedef typename DdtrTraits::value_type NumericalRep;
        typedef typename DdtrTraits::BeamConfigType BeamConfigType;

    public:
        Ddtr(BeamConfigType const& beam_config, ddtr::Config const& algo_config);
        Ddtr(Ddtr const&) = delete;
        Ddtr(Ddtr&&);
        ~Ddtr();

        /**
         * @brief dedispersion of time frequency data on CPU
         * @details DmTialsHandler called when buffer is full.
         * @param data   A TimeFrequency block and cpu resource
         * @return DmTime sequence i.e. timeseries for each DM trial value
         */
        std::shared_ptr<DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>& cpu
                                               , std::shared_ptr<BufferType> data
                                               );

        /**
         * @brief dedispersion of time frequency data on CPU
         * @details DmTialsHandler called when buffer is full.
         * @param data   A TimeFrequency block and cpu resource
         * @param call_back   A function that is called after the ddtr (most a SPS call)
         * @return DmTime sequence i.e. timeseries for each DM trial value
         */
        template <typename CallBackT>
        std::shared_ptr<DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>& cpu
                                               , std::shared_ptr<BufferType> data
                                               , CallBackT const& call_back);


        /**
         * @brief set a new dedispersion plan
         */
        void plan(DedispersionPlan const& plan);

    private:
        static_assert(std::is_empty<Worker<DdtrTraits>>::value, "If you are adding state to Worker then you will need to use DeviceLocal for thread safety");
        Worker<DdtrTraits> _worker;
        std::shared_ptr<DedispersionPlan> _plan;
};


} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/cpu/detail/Ddtr.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_CPU_DDTR_H
