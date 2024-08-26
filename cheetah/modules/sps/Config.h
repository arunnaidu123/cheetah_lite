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
#ifndef SKA_CHEETAH_MODULES_SPS_CONFIG_H
#define SKA_CHEETAH_MODULES_SPS_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/modules/sps/cpu/Config.h"
#include "cheetah/modules/sps/klotski_common/Config.h"
#include "cheetah/modules/sps/klotski_bruteforce/Config.h"
#include "cheetah/modules/sps/klotski/Config.h"
#include "cheetah/modules/ddtr/RfiExcisionConfig.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "panda/ProcessingEngine.h"
#include "panda/ResourcePool.h"
#include "panda/arch/nvidia/Nvidia.h"
#include "panda/PoolSelector.h"
#include "panda/MultipleConfigModule.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {

typedef panda::MultipleConfigModule<utils::Config
                                  , cpu::Config
                                  , klotski_bruteforce::Config
                                  , klotski::Config
                                  > SpsAlgoConfigs;
/**
 * @brief
 *   all non-templated options for the sps module
 */
class Config : public SpsAlgoConfigs
{
        typedef SpsAlgoConfigs BaseT;

    public:
        typedef data::DedispersionMeasureType<float> Dm;

    public:
        Config(ddtr::Config&);
        operator ddtr::Config const& () const;

        /**
         * @brief single pulse detection threshold in sigmas
         */
        float threshold() const;

        /**
         * @brief return the astroaccelerate algorithms configuration specifics
         */
        cpu::Config const& cpu_config() const;
        cpu::Config& cpu_config();
        klotski_bruteforce::Config const& klotski_bruteforce_config() const;
        klotski_bruteforce::Config& klotski_bruteforce_config();
        klotski::Config const& klotski_config() const;
        klotski::Config& klotski_config();

        ddtr::Config const& ddtr_config() const;
        ddtr::Config& ddtr_config();

        /**
         * @brief maximum number of samples to dedisperse
         * @details algorithms should restrict themselves to dispersing no more than this number
         *          of samples per pass. Pass through function for same method in the ddtr::Config
         */
        std::size_t dedispersion_samples() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        ddtr::Config& _ddtr_config;
        float _threshold;
};

// defines a templated ConfigType for Sps that adds on pool selection
template<typename PoolManagerType>
using ConfigType = panda::PoolSelector<PoolManagerType, Config>;

} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_SPS_CONFIG_H
