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
#include "cheetah/pipelines/search_pipeline/Empty.h"
#include "cheetah/utils/TerminateException.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


template<typename NumericalT>
Empty<NumericalT>::Empty(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config)
    : BaseT(config, beam_config)
    , _count(0)
    , _exit_after(config.empty_config().exit_after())
{
}

template<typename NumericalT>
Empty<NumericalT>::~Empty()
{
}

template<typename NumericalT>
void Empty<NumericalT>::operator()(TimeFrequencyType& data)
{
    this->out().send(panda::ChannelId("raw"), data);
    if((_exit_after == 0) || (_exit_after > 0 && ++_count <= _exit_after)) return;
    this->halt();
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
