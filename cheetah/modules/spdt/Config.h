/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_SPDT_CONFIG_H
#define SKA_CHEETAH_MODULES_SPDT_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/modules/spdt/cpu/Config.h"
#include "cheetah/modules/spdt/klotski/Config.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "panda/ProcessingEngine.h"
#include "panda/ResourcePool.h"
#include "panda/arch/nvidia/Nvidia.h"
#include "panda/PoolSelector.h"
#include "panda/MultipleConfigModule.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {

typedef panda::MultipleConfigModule<utils::Config
                                  , cpu::Config
                                  , klotski::Config
                                   > SpdtAlgoConfigs;
/**
 * @brief
 *   all non-templated options for the spdt module
 */
class Config : public SpdtAlgoConfigs
{
        typedef SpdtAlgoConfigs BaseT;

    public:
        typedef data::DedispersionMeasureType<float> Dm;

    public:
        Config();

        /**
         * @brief single pulse detection threshold in sigmas
         */
        float threshold() const;

        /**
         * @brief return the astroaccelerate algorithms configuration specifics
         */
        cpu::Config const& cpu_config() const;
        cpu::Config& cpu_config();

        /**
         * @brief return the astroaccelerate algorithms configuration specifics
         */
        klotski::Config const& klotski_config() const;
        klotski::Config& klotski_config();

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        float _threshold;
};

// defines a templated ConfigType for Spdt that adds on pool selection
template<typename PoolManagerType>
using ConfigType = panda::PoolSelector<PoolManagerType, Config>;

} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_SPDT_CONFIG_H
