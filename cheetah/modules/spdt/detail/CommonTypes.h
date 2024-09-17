/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_SPDT_COMMONTYPES_H
#define SKA_CHEETAH_MODULES_SPDT_COMMONTYPES_H

#include "cheetah/modules/ddtr/detail/RfiExcisionFactory.h"
#include "cheetah/modules/ddtr/detail/CommonTypes.h"
#include "cheetah/data/SpCcl.h"
#include "panda/AggregationBufferFiller.h"
#include <functional>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {

/**
 * @brief Contains common types used through spdt
 * @details
 */
/**
 * @brief Contains common types used through sps
 * @details
 */

template<typename ConfigType, typename NumericalT>
struct CommonTypes : public ddtr::CommonTypes<ConfigType, NumericalT>
{
    private:
        typedef ddtr::CommonTypes<ConfigType, NumericalT> BaseT;

    public:
        typedef ConfigType Config;
        typedef NumericalT NumericalRep;
        typedef NumericalT value_type;

        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef typename BaseT::BufferFillerType BufferFillerType;
        typedef typename BufferFillerType::AggregationBufferType BufferType;
        typedef typename BaseT::DmTrialsType DmTrialsType;
        typedef data::SpCcl<NumericalRep> SpType;

        typedef std::function<void(std::shared_ptr<SpType>)> SpdtHandler;
        typedef typename BaseT::DedispersionHandler DedispersionHandler;
        typedef typename pipelines::search_pipeline::BeamConfigType<NumericalRep> BeamConfigType;
};


} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_SPDT_COMMONTYPES_H
