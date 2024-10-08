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
#ifndef SKA_CHEETAH_MODULES_SPDT_KLOTSKI_SPDT_H
#define SKA_CHEETAH_MODULES_SPDT_KLOTSKI_SPDT_H

#ifdef SKA_CHEETAH_ENABLE_NASM

#include "cheetah/modules/ddtr/klotski/Ddtr.h"
#include "cheetah/modules/spdt/klotski/Config.h"
#include "cheetah/modules/spdt/detail/CommonTypes.h"
#include "cheetah/modules/spdt/Config.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/utils/Architectures.h"
#include "cheetah/utils/AlgorithmBase.h"
#include "cheetah/modules/ddtr/klotski/DedispersionPlan.h"
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace klotski {

/**
 * @brief Single pulse search on AVX
 * @details This not a asynchronous version
 */

namespace detail {

template<class SpdtTraits, typename Enable=void>
class Spdt
{
    public:
        typedef cheetah::Cpu Architecture;
        typedef klotski::Config Config;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;

    public:
        Spdt(spdt::Config const&) {};

        std::shared_ptr<typename SpdtTraits::DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>&, std::shared_ptr<DmTrialsType>);
};

} // namespace detail

template<class SpdtTraits>
class Spdt : public detail::Spdt<SpdtTraits>
{
        typedef detail::Spdt<SpdtTraits> BaseT;

    public:
        typedef typename BaseT::Architecture Architecture;
        typedef typename BaseT::DedispersionPlan DedispersionPlan;
        typedef klotski::Config Config;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;

    public:
        Spdt(spdt::Config const& config);

        std::shared_ptr<typename SpdtTraits::SpType> operator()(panda::PoolResource<Architecture>& dev, std::shared_ptr<DmTrialsType>);

};

} // namespace klotski
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/spdt/klotski/detail/Spdt.cpp"

#endif // SKA_CHEETAH_ENABLE_NASM
#endif // SKA_CHEETAH_MODULES_SPDT_KLOTSKI_SPDT_H
