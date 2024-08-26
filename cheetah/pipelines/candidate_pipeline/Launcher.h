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
#ifndef SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_LAUNCHER_H
#define SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_LAUNCHER_H

#include "PipelineFactory.h"
#include "Config.h"
#include "cheetah/io/exporters/DataExportConfig.h"
#include "cheetah/producers/Factory.h"
#include "cheetah/pipelines/search_pipeline/DataExport.h"
#include "panda/Launcher.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace candidate_pipeline {

template<typename NumericalT>
struct LauncherTraits {
    typedef data::SpCandidateData<data::TimeFrequency<Cpu, NumericalT>> SpCandidateDataType;
    typedef io::exporters::DataExportConfig SinkConfig;
    typedef pipelines::search_pipeline::DataExport<NumericalT, pipelines::search_pipeline::DefaultExportTraits> Sink;
    typedef producers::Config<SpCandidateDataType> SourceConfig;
    typedef producers::Factory<SpCandidateDataType> SourceFactory;
    typedef PipelineFactory<NumericalT> ComputeFactory;
    typedef candidate_pipeline::Config Config;
};

/**
 * @brief Launches the single pulse candidate processing pipeline
 */
template<typename NumericalT>
class Launcher : public panda::Launcher<LauncherTraits<NumericalT>>
{
        typedef panda::Launcher<LauncherTraits<NumericalT>> BaseT;

    public:
        Launcher();
};

} // namespace candidate_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/Launcher.cpp"

#endif // SKA_CHEETAH_PIPELINES_CANDIDATE_PIPELINE_LAUNCHER_H
