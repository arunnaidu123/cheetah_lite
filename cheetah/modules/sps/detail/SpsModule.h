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
 * @brief Generate the appropriate DDTR module for the selected SPS algorithms
 */

template<class SpsTraits, template<typename> class... SpsAlgorithms>
struct DdtrModuleGen;

/**
 * @brief specialization to generate DdtrModul when the SpsAlgorithms are more than one
 */
template<class SpsTraits, template<typename> class SpsAlgorithm, template<typename> class... SpsAlgorithms>
struct DdtrModuleGen<SpsTraits, SpsAlgorithm, SpsAlgorithms...>
{
    private:
        typedef typename DdtrModuleGen<SpsTraits, SpsAlgorithm>::type T1;
        typedef typename DdtrModuleGen<SpsTraits, SpsAlgorithms...>::type T2;

        template<typename DdtrModT1, typename DdtrModT2>
        struct Merge;
        template<template<typename> class AlgoTmp, template<typename> class... AlgoTmps>
        struct Merge<ddtr::DdtrModule<SpsTraits, AlgoTmp>
                    ,ddtr::DdtrModule<SpsTraits, AlgoTmps...>>
        {
            typedef ddtr::DdtrModule<SpsTraits, AlgoTmp, AlgoTmps...> type;
        };

    public:
        typedef typename Merge<T1, T2>::type type;
};

/**
 * @brief Called for single SpsAlgorithm.
 */
template<class SpsTraits, template<typename> class SpsAlgorithm>
struct DdtrModuleGen<SpsTraits, SpsAlgorithm>
{
    template<typename TraitsT>
    using SpsWrapperT = SpsWrapper<SpsAlgorithm<TraitsT>, TraitsT>;

    typedef ddtr::DdtrModule<SpsTraits, SpsWrapperT> type;
};

} // namespace detail


/**
 * @brief An sps module configurable with a number of arguments
 * @details Uses the Dddr::DdtrModule class to handle all the necessary buffering
 *          A full buffer will then callback to the sps algorithm (via SpsWrapper). The
 *          dm_handler is called with the return value of this function (by the Ddtr module).
 */

template<typename SpsTraits, template<typename> class... SpsAlgorithms>
class SpsModule
{
        typedef typename detail::DdtrModuleGen<SpsTraits, SpsAlgorithms...>::type DdtrT;
        typedef typename SpsTraits::SpHandler SpHandler;
        typedef typename SpsTraits::DedispersionHandler DedispersionHandler;
        typedef typename SpsTraits::Config Config;

    public:
        SpsModule(Config const& config, DedispersionHandler const& dm_handler, SpHandler const& sp_handler);

        template<typename DataT>
        void operator()(DataT&& data);

    private:
        SpHandler _sp_handler;
        DdtrT _ddtr;
};


} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/sps/detail/SpsModule.cpp"

#endif // SKA_CHEETAH_MODULES_SPS_SPSMODULE_H
