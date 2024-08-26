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
#ifndef SKA_CHEETAH_MODULES_SPS_SPS_H
#define SKA_CHEETAH_MODULES_SPS_SPS_H

#include "cheetah/modules/sps/Config.h"
#include "cheetah/modules/sps/detail/CommonTypes.h"
#include "cheetah/modules/sps/detail/SpsModule.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/modules/sps/cpu/Sps.h"
#include "cheetah/modules/sps/klotski_bruteforce/Sps.h"
#include "cheetah/modules/sps/klotski/Sps.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {

/**
 * @brief algorithms listed here will be made avaialble via the top level Sps API
 */
template<typename ConfigType, typename NumericalT>
using SpsAlgos=SpsModule<CommonTypes<ConfigType, NumericalT>
                               , cpu::Sps
#ifdef SKA_CHEETAH_ENABLE_NASM
                               , klotski_bruteforce::Sps
                               , klotski::Sps
#endif // SKA_CHEETAH_ENABLE_NASM
                               >;

/**
 * @brief SPS module top level API.
 * @details Exposes all available SPS algorithms to the user for selection via runtime configuration options
 */

template<class ConfigType, typename NumericalT>
class Sps : public SpsAlgos<ConfigType, NumericalT>
{
        typedef SpsAlgos<ConfigType, NumericalT> BaseT;
        typedef CommonTypes<ConfigType, NumericalT> SpsTraits;

    public:
        typedef data::TimeFrequency<Cpu, NumericalT> TimeFrequencyType;
        typedef typename SpsTraits::SpType SpType;
        typedef typename SpsTraits::SpHandler SpHandler;
        typedef typename SpsTraits::DmTrialsType DmTrialType;

    public:
        /**
         * @brief Constructor takes config and sp_handler
         * @details passes the arguments to BaseT
         * @param sp_handler A functor to be called with the Sps results. Its signature should be '''void(std::shared_ptr<data::SpCcl<NumericalT>>)'''.
         */
        Sps(ConfigType const& config, SpHandler const& sp_handler);
        ~Sps();

        /**
         * @brief operator which takes in DmTrails object to search for candidates in DMTime space.
         *
         * @param data A shared_ptr of DmTrailsType data type.
         */
        void operator()(std::shared_ptr<DmTrialType> const& data);

};

} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/sps/detail/Sps.cpp"

#endif // SKA_CHEETAH_MODULES_SPS_SPS_H
