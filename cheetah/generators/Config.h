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
#ifndef SKA_CHEETAH_GENERATORS_CONFIG_H
#define SKA_CHEETAH_GENERATORS_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include "cheetah/generators/BasebandGaussianNoiseConfig.h"
#include "cheetah/generators/DispersedPulseConfig.h"
#include "cheetah/generators/PulsarInjectionConfig.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *    Generator Configuration file
 *
 * @details
 * 
 */

class Config : public cheetah::utils::Config
{
    public:
        typedef panda::ConfigModuleIteratorWrapper<PulsarInjectionConfig> PulsarInjectionIterartorType;

    public:
        Config();
        ~Config();

        GaussianNoiseConfig const& gaussian_noise() const;
        BasebandGaussianNoiseConfig const& baseband_gaussian_noise() const;
        /// access the pulsar iteration configuration options
        PulsarInjectionIterartorType pulsar_injection_begin() const;
        PulsarInjectionIterartorType pulsar_injection_end() const;

        DispersedPulseConfig const& dispersed_pulse() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        GaussianNoiseConfig _gaussian_noise;
        BasebandGaussianNoiseConfig _baseband_gaussian_noise;
        DispersedPulseConfig _dispersed_pulse;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_CONFIG_H
