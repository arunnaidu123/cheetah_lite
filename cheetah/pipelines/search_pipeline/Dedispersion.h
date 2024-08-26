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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_DEDISPERSION_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_DEDISPERSION_H

#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"
#include "cheetah/modules/sps/Sps.h"
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
 * @brief Base class for dedispersion pipeline handlers
 */

template<typename NumericalT>
class Dedispersion : public PipelineHandler<NumericalT>
{
        typedef PipelineHandler<NumericalT> BaseT;
        typedef modules::sps::Sps<modules::sps::ConfigType<typename CheetahConfig<NumericalT>::PoolManagerType>, NumericalT> Sps;

    public:
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef typename Sps::DmHandler DmHandler;
        typedef typename std::shared_ptr<typename Sps::DmTrialType> DmTrialType;
        typedef NumericalT NumericalRep;

    public:
        Dedispersion(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config, DmHandler const& dm_handler = [](DmTrialType const&){});
        ~Dedispersion() = 0;

        void operator()(TimeFrequencyType&) override = 0;

    protected:
        DmHandler _dm_handler;
};


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/Dedispersion.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_DEDISPERSION_H
