/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIMACCELERATIONSEARCHIMPL_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIMACCELERATIONSEARCHIMPL_H

#include "cheetah/pipelines/search_pipeline/detail/AccelerationSearchImpl.h"
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
 * @brief An Acceleration Search Pipeline With RFI Mitigation
 * @details
 */

template<typename NumericalT, typename AccelerationSearchTraitsT, template<typename> class RfimPolicyTemplate>
class RfimAccelerationSearchImpl : public AccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT>
{
    public:
        typedef AccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT> BaseT;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef RfimPolicyTemplate<TimeFrequencyType> RfimPolicy;
        typedef typename modules::rfim::PolicyInfo<RfimPolicy>::ReturnType RfimReturnType;

    public:
        RfimAccelerationSearchImpl(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config);
        RfimAccelerationSearchImpl(RfimAccelerationSearchImpl&&);
        ~RfimAccelerationSearchImpl();

        void operator()(TimeFrequencyType&) override;

    private:
        class RfimHandler {
            public:
                RfimHandler(RfimAccelerationSearchImpl&);
                void operator()(RfimReturnType ) const;

            private:
                RfimAccelerationSearchImpl& _pipeline;
        };

        typedef RfiDetectionPipeline<NumericalT, RfimHandler, RfimPolicy> RfimType;

    private:
        RfimType _rfim;
};


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "RfimAccelerationSearchImpl.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_RFIMACCELERATIONSEARCHIMPL_H
