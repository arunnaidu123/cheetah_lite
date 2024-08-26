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
#ifndef SKA_CHEETAH_MODULES_DDTR_CPU_WORKER_H
#define SKA_CHEETAH_MODULES_DDTR_CPU_WORKER_H

#include "cheetah/modules/ddtr/cpu/DedispersionPlan.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

/**
 * @brief detail class that matches the workload of a single thread
 */

template<typename DdtrTraits>
class Worker
{
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef typename DdtrTraits::BufferType BufferType;
        typedef typename DdtrTraits::value_type NumericalT;
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef typename data::FrequencyTime<Cpu, NumericalT> FrequencyTimeType;


    public:
        Worker();
        // perform ddtr. Note this requires DedispersionPlan to be stable inside this fn call
        template<typename CallBackT>
        std::shared_ptr<DmTrialsType> operator()(BufferType const& data
                                                , std::shared_ptr<DedispersionPlan<DdtrTraits>> plan
                                                , CallBackT const& callback);

};


} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/ddtr/cpu/detail/Worker.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_CPU_WORKER_H
