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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSE_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSE_H

#include "detail/SinglePulseImpl.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class CheetahConfig;

template<typename NumericalT>
class BeamConfig;

/**
 * @brief A Single Pulse Search Pipeline
 * @details Uses config parameters to construct a suitably optimised SinglePulse pipeline
 */

template<typename NumericalT>
class SinglePulse : public SinglePulseImpl<NumericalT>
{
        typedef SinglePulseImpl<NumericalT> BaseT;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef typename BaseT::DmTrialType DmTrialType;
        typedef typename BaseT::BufferType BufferType;

    public:
        SinglePulse(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config);
        SinglePulse(SinglePulse&&);
        ~SinglePulse();

        void operator()(TimeFrequencyType&) override;

    private:
        std::unique_ptr<SinglePulseImpl<NumericalT>> _spimpl;
};


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/SinglePulse.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSE_H
