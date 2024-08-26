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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_EMPTY_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_EMPTY_H


#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class CheetahConfig;

template<typename NumericalT>
class BeamConfig;

/**
 * @brief
 *   A do nothing handler
 */
template<typename NumericalT>
class Empty : public PipelineHandler<NumericalT>
{
        typedef PipelineHandler<NumericalT> BaseT;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;

    public:
        Empty(CheetahConfig<NumericalT> const&, BeamConfig<NumericalT> const&);
        ~Empty() override;

        /**
         * @brief called whenever data is available for processing
         */
        void operator()(TimeFrequencyType&) override;

    private:
        std::size_t _count;
        std::size_t _exit_after;

        typedef std::chrono::high_resolution_clock ClockType;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/Empty.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_EMPTY_H
