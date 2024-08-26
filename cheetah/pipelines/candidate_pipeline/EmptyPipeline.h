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
#ifndef SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_EMPTYPIPELINE_H
#define SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_EMPTYPIPELINE_H


#include "ProcessingPipeline.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace candidate_pipeline {

/**
 * @brief A simple pass through pipeline that perform no compute tasks
 * @details
 */

template<typename NumericalT>
class EmptyPipeline : public ProcessingPipeline<NumericalT>
{
        typedef ProcessingPipeline<NumericalT> BaseT;
        typedef typename BaseT::SpCandidateDataType SpCandidateDataType;

    public:
        EmptyPipeline(Config const& config, typename BaseT::Exporter& exporter);
        ~EmptyPipeline() = default;

        void operator()(SpCandidateDataType&) override;
};

} // namespace candidate_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/EmptyPipeline.cpp"

#endif // SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_EMPTYPIPELINE_H
