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
#ifndef SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_PIPELINEFACTORY_H
#define SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_PIPELINEFACTORY_H

#include "Config.h"
#include "ProcessingPipeline.h"
#include "panda/Factory.h"


namespace ska {
namespace cheetah {
namespace pipelines {
namespace candidate_pipeline {

/**
 * @brief generates compute pipelines of the correct type corresponding to a type description string
 */
template<typename NumericalT>
class PipelineFactory : public ska::panda::Factory<ProcessingPipeline<NumericalT>>
{
        typedef ska::panda::Factory<ProcessingPipeline<NumericalT>> BaseT;

    public:
        typedef NumericalT NumericalRep;
        typedef ProcessingPipeline<NumericalRep> PipelineType;

    public:
        PipelineFactory(Config& config, typename PipelineType::Exporter& exporter);

    private:
        Config const& _config;
        typename PipelineType::Exporter& _sink;
};

} // namespace candidate_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/PipelineFactory.cpp"

#endif // SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_PIPELINEFACTORY_H