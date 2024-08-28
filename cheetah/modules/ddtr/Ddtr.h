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
#ifndef SKA_CHEETAH_MODULES_DDTR_DDTR_H
#define SKA_CHEETAH_MODULES_DDTR_DDTR_H

#include "cheetah/Configuration.h"
#include "cheetah/modules/ddtr/detail/CommonTypes.h"
#include "cheetah/modules/ddtr/detail/TimeFrequencyBufferFactory.h"
#include "cheetah/modules/ddtr/detail/DdtrModule.h"
#include "cheetah/modules/ddtr/detail/CommonDedispersionPlan.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/modules/ddtr/cpu/Ddtr.h"

#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename FunctorType, typename... Args> class Method {};

/**
 * @brief algorithms listed here will be made avaialble via the top level Ddtr API
 * @details Note that the ddtr::Config class should also needs to support the algorithms
 *          specific configuration object as given by the my_algo::Ddtr::Config typedef.
 */
template<typename ConfigType, typename NumericalRep, template<typename> class AggregationBufferFactoryTemplate>
using DdtrAlgos=DdtrModule<CommonTypes<ConfigType, NumericalRep, AggregationBufferFactoryTemplate>
                           , cpu::Ddtr
                       >;

/**
 * @brief DDTR module top level API.
 * @details Exposes all available ddtr algorithms to the user for selection via runtime configuration options
 */
template<typename ConfigType, typename NumericalRep, template<typename> class AggregationBufferFactoryTemplate=TimeFrequencyBufferFactory>
class Ddtr : public DdtrAlgos<ConfigType, NumericalRep, AggregationBufferFactoryTemplate>
{
        typedef DdtrAlgos<ConfigType, NumericalRep, AggregationBufferFactoryTemplate> BaseT;
        typedef CommonTypes<ConfigType, NumericalRep, AggregationBufferFactoryTemplate> DdtrTraits;

    public:
        typedef typename DdtrTraits::DedispersionHandler DedispersionHandler;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;

    public:
        Ddtr(ConfigType const& config, DedispersionHandler);
        ~Ddtr();

        /**
         * @brief dedisperses chunk of buffer data to a dm-time chunk.
         *
         * @details the DedispersionHandler will be called when dedispersion is complete. The method
         *         is delegated device specific implementations.
         *
         * @param[in] input A TimeFequency data type (or equivalent) of data to dedisperse.
         */
        template<typename TimeFreqDataT
                , typename data::EnableIfIsTimeFrequency<TimeFreqDataT, bool> = true>
        void operator()(TimeFreqDataT const& input);

        /**
         * @brief accept shared_ptr of TimeFrequency data types
         * @details calls the appropriate operator for type T if it exists
         */
        template<typename T>
        void operator()(std::shared_ptr<T> const& data);

};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/detail/Ddtr.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_DDTR_H
