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
#ifndef SKA_CHEETAH_MODULES_DDTR_CONFIG_H
#define SKA_CHEETAH_MODULES_DDTR_CONFIG_H
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include "cheetah/modules/ddtr/cpu/Config.h"
#include "cheetah/modules/ddtr/klotski/Config.h"
#include "cheetah/modules/ddtr/klotski_bruteforce/Config.h"
#include "cheetah/modules/ddtr/RfiExcisionConfig.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include "panda/PoolSelector.h"
#include "panda/MultipleConfigModule.h"
#include <cstdlib>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief add new ddtr algos Config types here to be included in the Ddtr::Config object
 * @details access the config type you want with a call to the templated config method
 * @code
 *    ddtr::Config ddtr_config;
 *    ddtr::cpu::Config const& cpu_config =  ddtr_config.config<cpu::Config>();
 * @endcode
 */
typedef panda::MultipleConfigModule<DedispersionTrialPlan
                                  , cpu::Config
                                  , klotski::Config
                                  , klotski_bruteforce::Config
                                  > DdtrAlgoConfigs;

class Config : public DdtrAlgoConfigs
{
        typedef DdtrAlgoConfigs BaseT;

    public:
        typedef data::DedispersionMeasureType<float> Dm;
        typedef boost::units::quantity<data::dm_constant::s_mhz::Unit, double> DmConstantType;

    public:
        Config();
        Config(Config const&) = delete;
        ~Config();

        /**
         * @brief return the cpu algorithm configuration parameters
         */
        cpu::Config const& cpu_algo_config() const;
        cpu::Config& cpu_algo_config();

        /**
         * @brief return the klotski algorithm configuration parameters
         */
        klotski::Config const& klotski_algo_config() const;
        klotski::Config& klotski_algo_config();

        /**
         * @brief return the klotski_bruteforce algorithm configuration parameters
         */
        klotski_bruteforce::Config const& klotski_bruteforce_algo_config() const;
        klotski_bruteforce::Config& klotski_bruteforce_algo_config();

        RfiExcisionConfig const& rfiexcision_config() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        RfiExcisionConfig _rfi_excision_config;

};

template<typename PoolManagerType>
using ConfigType = panda::PoolSelector<PoolManagerType, Config>;

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_CONFIG_H
