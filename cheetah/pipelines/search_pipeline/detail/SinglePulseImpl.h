/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSEIMPL_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSEIMPL_H

#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"
#include "cheetah/pipelines/search_pipeline/Dedispersion.h"
#include "cheetah/modules/sps/Sps.h"
#include "cheetah/modules/spsift/SpSift.h"
#include "cheetah/modules/sps_clustering/SpsClustering.h"
#include "panda/Thread.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class CheetahConfig;

template<typename NumericalT>
class BeamConfig;

/**
 * @brief A Single Pulse Search Pipeline without rfim
 */

template<typename NumericalT>
class SinglePulseImpl : public Dedispersion<NumericalT>
{
    public:
        typedef Dedispersion<NumericalT> BaseT;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef modules::sps::Sps<modules::sps::ConfigType<typename CheetahConfig<NumericalT>::PoolManagerType>, NumericalT> Sps;
        typedef typename Sps::DmHandler DmHandler;
        typedef typename Sps::DmTrialType DmTrialType;

    protected:
        typedef typename Sps::SpType SpType;

    public:
        SinglePulseImpl(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config, DmHandler const& dm_handler = [](std::shared_ptr<DmTrialType>){});
        SinglePulseImpl(SinglePulseImpl&&);
        ~SinglePulseImpl();

        void operator()(TimeFrequencyType& data) override;

    private:
        void do_post_processing(std::shared_ptr<SpType> const& data);

    protected:
        class SpsHandler {
            public:
                SpsHandler(SinglePulseImpl&);
                SpsHandler(SpsHandler const&) = delete;
                void operator()(std::shared_ptr<SpType> const&) const;

            private:
                SinglePulseImpl& _pipeline;
        };

    private:
        SpsHandler _sps_handler;

    private:
        modules::sps_clustering::SpsClustering _spclusterer;
        modules::spsift::SpSift _spsifter;
        panda::Thread _thread;

    protected:
        Sps _sps;
};


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "SinglePulseImpl.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSEIMPL_H
