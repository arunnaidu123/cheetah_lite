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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIDETECTIONPIPELINE_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIDETECTIONPIPELINE_H

#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"
#include "cheetah/modules/rfim/Rfim.h"
#include "cheetah/modules/rfim/policy/LastUnflagged.h"
#include "cheetah/channel_mask/ConfigurableChannelMask.h"
#include "cheetah/channel_mask/PolicyFactory.h"
#include "panda/ModuloNumber.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class CheetahConfig;

template<typename NumericalT>
class BeamConfig;

namespace {
    class NullHandler
    {
        public:
            template<typename TimeFrequencyType>
            inline void operator()(TimeFrequencyType) {}
    };
}

/**
 * @brief
 *   Pipeline that performs only RFI detection
 */
template<typename NumericalT, typename RfimOutputHandler=NullHandler, typename RfiPolicy=modules::rfim::FlagPolicy<typename PipelineHandler<NumericalT>::TimeFrequencyType>>
class RfiDetectionPipeline : public PipelineHandler<NumericalT>
{
        typedef PipelineHandler<NumericalT> BaseT;

    public:
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;

    public:
        template<typename... HandlerArgs>
        RfiDetectionPipeline(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config, HandlerArgs&&...);
        virtual ~RfiDetectionPipeline();

        void operator()(TimeFrequencyType&) override;

        /// access to the output handler object
        RfimOutputHandler& output_handler();

    private:
        class RfiOutputHandler {
                typedef typename modules::rfim::PolicyInfo<RfiPolicy>::ReturnType ReturnType;

            public:
                template<typename... HandlerArgs>
                RfiOutputHandler(RfiDetectionPipeline&, HandlerArgs&&...);
                inline void operator()(ReturnType);

                RfiDetectionPipeline& _pipeline;
                RfimOutputHandler _output; // user defined RfimOutputHandler
        };

        class BandPassOutputHandler {
                typedef modules::rfim::ampp::Spectrum<NumericalT> BandPassType;

            public:
                BandPassOutputHandler(RfiDetectionPipeline&);
                inline void operator()(BandPassType const&);

            private:
                RfiDetectionPipeline& _pipeline;
        };

        struct RfimTraits {
            typedef RfiOutputHandler RfimHandler;
            typedef BandPassOutputHandler BandPassHandler;
            typedef RfiPolicy Policy;
            typedef NumericalT NumericalRep;
        };

        typedef modules::rfim::Rfim<TimeFrequencyType, RfimTraits> RfimType;

    public:
        typedef typename RfimType::ReturnType ReturnType;

    private:
        RfiOutputHandler _rfim_handler;
        BandPassOutputHandler _bandpass_handler;
        RfimType _rfim;
        std::vector<TimeFrequencyType*> _data_sequence;
        panda::AtomicModuloNumber<unsigned, std::memory_order_relaxed> _last_data_sequence_index;
        panda::AtomicModuloNumber<unsigned, std::memory_order_relaxed> _next_data_sequence_index;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/RfiDetectionPipeline.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIDETECTIONPIPELINE_H
