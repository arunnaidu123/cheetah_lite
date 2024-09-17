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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_MULTIBEAMCONFIG_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_MULTIBEAMCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include <string>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 * @brief
 *    Node to access multiple beam configurations
 */

template<typename NumericalT>
class MultiBeamConfig : public utils::Config
{
    public:
        typedef utils::Config BaseT;
        typedef typename BaseT::PoolManagerType PoolManagerType;
        typedef panda::ConfigModuleIteratorWrapper<BeamConfigType<NumericalT>> ConstIterator;

    public:
        MultiBeamConfig(PoolManagerType&);
        ~MultiBeamConfig();

        /**
         * @brief return the iterator over multiple beam subsections
         */
        ConstIterator beams() const;

        /**
         * @brief return the end iterator
         */
        ConstIterator beams_end() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        PoolManagerType& _pool_manager;

};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/MultiBeamConfig.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_MULTIBEAMCONFIG_H
