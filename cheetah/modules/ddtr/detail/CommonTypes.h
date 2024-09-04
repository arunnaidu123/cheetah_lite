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
#ifndef SKA_CHEETAH_MODULES_DDTR_COMMONTYPES_H
#define SKA_CHEETAH_MODULES_DDTR_COMMONTYPES_H

#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include "AggregationBufferFiller.h"
#include <functional>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief Contains common types used through ddtr
 * @details
 */
template<typename ConfigType, typename NumericalT>
struct CommonTypes
{
    typedef ConfigType Config;
    typedef typename ConfigType::PoolType Pool;
    typedef NumericalT NumericalRep;
    typedef NumericalRep value_type;
    typedef data::TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef AggregationBufferFiller<NumericalRep> BufferFillerType;
    typedef typename BufferFillerType::AggregationBufferType BufferType;
    typedef data::DmTrials<Cpu, float> DmTrialsType;
    typedef std::function<void(std::shared_ptr<DmTrialsType>)> DedispersionHandler;
    typedef typename pipelines::search_pipeline::BeamConfig<NumericalRep> BeamConfigType;
};


} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_COMMONTYPES_H
