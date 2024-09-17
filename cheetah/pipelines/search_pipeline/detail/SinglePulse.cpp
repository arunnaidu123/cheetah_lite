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

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
SinglePulse<NumericalT>::SinglePulse(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config)
    : BaseT(config, beam_config)
{
    SinglePulseFactory<NumericalT> factory(config);
    _spimpl.reset(factory.create(beam_config));
}

template<typename NumericalT>
SinglePulse<NumericalT>::SinglePulse(SinglePulse&& s)
    : _spimpl(s._spimpl)
{
}

template<typename NumericalT>
SinglePulse<NumericalT>::~SinglePulse()
{
}

template<typename NumericalT>
inline
void SinglePulse<NumericalT>::operator()(TimeFrequencyType& data)
{
    _spimpl->operator()(data);
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
