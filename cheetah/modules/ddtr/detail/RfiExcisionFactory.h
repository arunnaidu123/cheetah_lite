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
#ifndef SKA_CHEETAH_MODULES_DDTR_RFIEXCISIONFACTORY_H
#define SKA_CHEETAH_MODULES_DDTR_RFIEXCISIONFACTORY_H

#include "cheetah/modules/ddtr/detail/TimeFrequencyBufferFactory.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/RfimFlaggedData.h"
#include "cheetah/modules/ddtr/detail/RfiExcision.h"
#include "cheetah/modules/ddtr/RfiExcisionConfig.h"
#include "panda/AggregationBufferFiller.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief RFI Factory interface
 * @details This goes as an input to the Agregation object contructor.
 */
template<typename TimeFrequencyType>
class RfiExcisionFactory: public ddtr::TimeFrequencyBufferFactory<TimeFrequencyType>
{
        typedef ddtr::TimeFrequencyBufferFactory<TimeFrequencyType> BaseT;

    public:
        typedef data::RfimFlaggedData<TimeFrequencyType> RfiFlagDataType;
        typedef RfiExcision<TimeFrequencyType, RfiFlagDataType> Iterator;
        typedef RfiExcision<const TimeFrequencyType, const RfiFlagDataType> ConstIterator;

    public:
        RfiExcisionFactory( RfiExcisionConfig const& config);
        ~RfiExcisionFactory();

        using BaseT::begin;
        using BaseT::end;

        inline Iterator begin(RfiFlagDataType& data);
        inline ConstIterator begin(RfiFlagDataType const& data);
        inline Iterator end(RfiFlagDataType& data);
        inline ConstIterator end(RfiFlagDataType const& data);

    private:
        RfiExcisionConfig const& _config;
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/ddtr/detail/RfiExcisionFactory.cpp"
#endif // SKA_CHEETAH_MODULES_DDTR_RFIEXCISIONFACTORY_H
