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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIMSINGLEPULSEIMPL_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIMSINGLEPULSEIMPL_H

#include "cheetah/pipelines/search_pipeline/detail/SinglePulseImpl.h"
#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"
#include "cheetah/pipelines/search_pipeline/RfiDetectionPipeline.h"
#include "cheetah/modules/rfim/PolicyInfo.h"
#include "cheetah/modules/rfim/Rfim.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class CheetahConfig;

template<typename NumericalT>
class BeamConfig;

/**
 * @brief A Single Pulse Search Pipeline With RFI Mitigation
 * @details
 */

template<typename NumericalT, template<typename> class RfimPolicyTemplate>
class RfimSinglePulseImpl : public SinglePulseImpl<NumericalT>
{
    public:
        typedef SinglePulseImpl<NumericalT> BaseT;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef typename BaseT::DmHandler DmHandler;
        typedef RfimPolicyTemplate<TimeFrequencyType> RfimPolicy;
        typedef typename modules::rfim::PolicyInfo<RfimPolicy>::ReturnType RfimReturnType;

    public:
        RfimSinglePulseImpl(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config, DmHandler const& dm_handler);
        RfimSinglePulseImpl(RfimSinglePulseImpl&&);
        ~RfimSinglePulseImpl();

        void operator()(TimeFrequencyType&) override;

    private:
        class RfimHandler {
            public:
                RfimHandler(RfimSinglePulseImpl&);
                void operator()(RfimReturnType) const;

            private:
                RfimSinglePulseImpl& _pipeline;
        };

        typedef RfiDetectionPipeline<NumericalT, RfimHandler, RfimPolicy> RfimType;

    private:
        RfimType _rfim;
};


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "RfimSinglePulseImpl.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIMSINGLEPULSEIMPL_H
