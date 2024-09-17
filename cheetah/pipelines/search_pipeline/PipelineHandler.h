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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_PIPELINEHANDLER_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_PIPELINEHANDLER_H

#include "cheetah/data/TimeFrequency.h"
#include "cheetah/pipelines/search_pipeline/DataExport.h"
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class BeamConfig;

/**
 * @brief
 *    Base class for Cheetah Pipeline Handlers
 */

template<typename NumericalT>
class PipelineHandler
{
    public:
        typedef data::TimeFrequency<Cpu, NumericalT> TimeFrequencyType;

    public:
        PipelineHandler(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config);
        virtual ~PipelineHandler() = 0;

        /**
         * @brief called each time data becomes available
         */

        virtual void operator()(TimeFrequencyType&) = 0;

        /**
         * @brief return the data output streamer
         * @details use the streamer to send your data to some external system(s) (e.g. file, network)
         * @code
         *    out().send(panda::ChannelId("channel_name"), myData);
         * @endcode
         */
        inline DataExport<NumericalT>& out() { return _out; }

        /**
         * @brief halt the pipeline
         */
        void halt();

    private:
        CheetahConfig<NumericalT> const& _config;
        DataExport<NumericalT>& _out;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/PipelineHandler.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_PIPELINEHANDLER_H
