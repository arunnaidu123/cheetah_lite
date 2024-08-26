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
#ifndef SKA_CHEETAH_MODULES_SPS_SPSMODULE_H
#define SKA_CHEETAH_MODULES_SPS_SPSMODULE_H

#include "cheetah/modules/sps/detail/SpsWrapper.h"
#include "cheetah/modules/sps/detail/SpsAlgoFactory.h"
#include "cheetah/modules/ddtr/detail/DdtrModule.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {

namespace detail {

/**
 * @brief An sps module configurable with a number of arguments
 */

template<typename SpsTraits, template<typename> class... SpsAlgorithms>
class SpsModule
{
        typedef typename detail::DdtrModuleGen<SpsTraits, SpsAlgorithms...>::type DdtrT;
        typedef typename SpsTraits::SpHandler SpHandler;
        typedef typename SpsTraits::Config Config;
        typedef typename SpsTraits::DmTrialsType DmTrialsType;

    public:
        SpsModule(Config const& config, SpHandler const& sp_handler);

        void operator()(shared_ptr<DmTrialsType> const& data);

    private:
        SpHandler _sp_handler;
};


} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/sps/detail/SpsModule.cpp"

#endif // SKA_CHEETAH_MODULES_SPS_SPSMODULE_H
