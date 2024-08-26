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
#include "cheetah/pipelines/search_pipeline/MultiBeamConfig.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

inline static const std::string& beam_tag()
{
    static std::string var("beam");
    return var;
}

template<typename NumericalT>
MultiBeamConfig<NumericalT>::MultiBeamConfig()
    : BaseT("beams")
{
    add_factory(beam_tag(), []() { return new BeamConfig<NumericalT>(); } );
}

template<typename NumericalT>
MultiBeamConfig<NumericalT>::~MultiBeamConfig()
{
}

template<typename NumericalT>
typename MultiBeamConfig<NumericalT>::ConstIterator MultiBeamConfig<NumericalT>::beams() const
{
    return subsection(beam_tag());
}

template<typename NumericalT>
typename MultiBeamConfig<NumericalT>::ConstIterator MultiBeamConfig<NumericalT>::beams_end() const
{
    return subsection_end();
}

template<typename NumericalT>
void MultiBeamConfig<NumericalT>::add_options(OptionsDescriptionEasyInit&)
{
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
