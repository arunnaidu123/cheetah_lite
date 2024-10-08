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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSEFACTORY_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSEFACTORY_H

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename>
class Dedispersion;

template<typename>
class CheetahConfig;

template<typename>
class BeamConfig;

/**
 * @brief Generate a suitable SinglePulse implementaion bases on the configuration parameters
 * @details
 */

template<typename NumericalT>
class SinglePulseFactory
{
    public:
        SinglePulseFactory(CheetahConfig<NumericalT> const& config);
        ~SinglePulseFactory();

        SinglePulseImpl<NumericalT>* create(BeamConfigType<NumericalT> const& beam_config) const;

    protected:
        template<template<typename> class RfimPolicyTempl>
        SinglePulseImpl<NumericalT>* create_policy(BeamConfigType<NumericalT> const& beam_config) const;

    private:
        CheetahConfig<NumericalT> const& _config;
};


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/SinglePulseFactory.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SINGLEPULSEFACTORY_H
