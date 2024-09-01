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

#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_BRUTEFORCE_DDTRWORKER_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_BRUTEFORCE_DDTRWORKER_H
#include "cheetah/Configuration.h"

#ifdef SKA_CHEETAH_ENABLE_NASM

#include "cheetah/modules/ddtr/klotski_bruteforce/Config.h"
#include "cheetah/modules/ddtr/klotski_bruteforce/detail/DedispersionStrategy.h"
#include "cheetah/modules/ddtr/klotski_bruteforce/DedispersionPlan.h"
#include "cheetah/modules/ddtr/DedispersionConfig.h"
#include "cheetah/utils/Architectures.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {

/**
 * @brief Manages the running of the threaded dedispersion
 */

template<typename DdtrTraits>
class DdtrWorker
{
    public:
        typedef cheetah::Cpu Architecture;
        typedef typename DdtrTraits::DmTrialsType DmTrialsType;
        typedef typename DdtrTraits::NumericalRep NumericalRep;

    public:
        DdtrWorker();

        /**
         * @brief call the dedispersion algorithm using the provided device
         */
        template<typename BufferType, typename CallBackT>
        std::shared_ptr<DmTrialsType> operator()(std::shared_ptr<BufferType>
                                               , std::shared_ptr<DedispersionPlan<DdtrTraits>>
                                               , CallBackT const&);

};


} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "DdtrWorker.cpp"

#endif // SKA_CHEETAH_ENABLE_NASM
#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_BRUTEFORCE_DDTRWORKER_H
