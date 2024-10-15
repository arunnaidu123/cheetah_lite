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
#ifndef SKA_CHEETAH_MODULES_SPDT_SPDT_H
#define SKA_CHEETAH_MODULES_SPDT_SPDT_H

#include "cheetah/modules/spdt/Config.h"
#include "cheetah/modules/spdt/detail/CommonTypes.h"
#include "cheetah/modules/spdt/detail/SpdtModule.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/modules/spdt/cpu/Spdt.h"
#include "cheetah/modules/spdt/klotski/Spdt.h"

#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {

/**
 * @brief algorithms listed here will be made avaialble via the top level spdt API
 */
template<typename BeamConfigType, typename ConfigType, typename NumericalT>
using SpdtAlgos=SpdtModule<CommonTypes<BeamConfigType, ConfigType, NumericalT>
                               , cpu::Spdt
#ifdef SKA_CHEETAH_ENABLE_NASM
                               , klotski::Spdt
#endif //SKA_CHEETAH_ENABLE_NASM
                               >;

/**
 * @brief Spdt module top level API.
 * @details Exposes all available Spdt algorithms to the user for selection via runtime configuration options
 */

template<class BeamConfigType, class ConfigType, typename NumericalT>
class Spdt : public SpdtAlgos<BeamConfigType, ConfigType, NumericalT>
{
        typedef SpdtAlgos<BeamConfigType, ConfigType, NumericalT> BaseT;
        typedef CommonTypes<BeamConfigType, ConfigType, NumericalT> SpdtTraits;

    public:
        typedef typename SpdtTraits::SpType SpType;
        typedef typename SpdtTraits::SpdtHandler SpdtHandler;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;
        typedef typename SpdtTraits::DmTrialsType DmTrialType;
        typedef typename SpdtTraits::DedispersionHandler DmHandler;
        typedef typename SpdtTraits::BufferType BufferType;
        typedef typename SpdtTraits::TimeFrequencyType TimeFrequencyType;

    public:
        /**
         * @brief Constructor takes config and sp_handler
         * @details passes the arguments to BaseT
         * @param sp_handler A functor to be called with the spdt results. Its signature should be '''void(std::shared_ptr<data::SpCcl<NumericalT>>)'''.
         */
        Spdt(BeamConfigType const& beam_config, ConfigType const& config, SpdtHandler const& sp_handler);
        ~Spdt();

        /**
         * @brief operator which takes in DmTrails object to search for candidates in DMTime space.
         *
         * @param data A shared_ptr of DmTrailsType data type.
         */
        void operator()(std::shared_ptr<DmTrialsType> data);

};

} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/spdt/detail/Spdt.cpp"

#endif // SKA_CHEETAH_MODULES_SPDT_SPDT_H
