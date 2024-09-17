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
#include "cheetah/pipelines/search_pipeline/SinglePulseFactory.h"
#include "cheetah/pipelines/search_pipeline/detail/SinglePulseImpl.h"
#include "cheetah/pipelines/search_pipeline/detail/RfimSinglePulseImpl.h"
#include "cheetah/modules/rfim/policy/FlagPolicy.h"
#include "cheetah/modules/rfim/policy/LastUnflagged.h"


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


template<typename NumericalT>
SinglePulseFactory<NumericalT>::SinglePulseFactory(CheetahConfig<NumericalT> const& config)
    : _config(config)
{
}

template<typename NumericalT>
SinglePulseFactory<NumericalT>::~SinglePulseFactory()
{
}

template<typename NumericalT>
template<template <typename> class RfimPolicyTempl>
SinglePulseImpl<NumericalT>* SinglePulseFactory<NumericalT>::create_policy(BeamConfigType<NumericalT> const& beam_config) const
{
    //if(!_config.rfim_config().algo_defined()) {
        return new SinglePulseImpl<NumericalT>(_config, beam_config);
    //}
    //else {
    //    return new RfimSinglePulseImpl<NumericalT, RfimPolicyTempl>(_config, beam_config, std::forward<DmHandler>(dm_handler));
    //}
}

template<typename NumericalT>
SinglePulseImpl<NumericalT>* SinglePulseFactory<NumericalT>::create(BeamConfigType<NumericalT> const& beam_config) const
{
    //if(!_config.rfim_config().algo_defined()) {
        return new SinglePulseImpl<NumericalT>(_config, beam_config);
    //}
    //else {
    //    if(_config.rfim_config().flag_policy()) {
    //        return create_policy<modules::rfim::FlagPolicy>(beam_config, std::forward<DmHandler>(dm_handler));
    //    }
    //    else {
    //        return create_policy<modules::rfim::LastUnflagged>(beam_config, std::forward<DmHandler>(dm_handler));
    //    }
    //}
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
