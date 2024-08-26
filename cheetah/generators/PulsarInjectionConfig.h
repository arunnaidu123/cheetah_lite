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
#ifndef SKA_CHEETAH_GENERATORS_PULSARINJECTIONCONFIG_H
#define SKA_CHEETAH_GENERATORS_PULSARINJECTIONCONFIG_H


#include "pulse_profile/PulsarProfileId.h"
#include "PhaseModelFactory.h"
#include "SimplePhaseModelConfig.h"
#include "Tempo2PhaseModelConfig.h"
#include "cheetah/utils/Config.h"
#include "cheetah/data/DedispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *    Confgiuration Parameters for the pulsar injection generator
 * @details
 */

class PulsarInjectionConfig : public utils::Config
{
    public:
        typedef PhaseModelFactory::type PhaseModelType;

    public:
        PulsarInjectionConfig(std::string const& tagname = "pulsar");
        ~PulsarInjectionConfig();

        /// signal to nosie ratio to emulate
        float signal_to_noise() const;

        /// set the target signal to nosie ratio to emulate
        void signal_to_noise(float snr);

        /**
         * @brief the name of the pulse profile to emulate
         */
        PulsarProfileId const& profile() const;

        /**
         * @brief set the name of the pulse profile to emulate
         */
        void profile(std::string const& profile_name);

        /**
         * @brief set the spectral index of the pulse
         */
        void spectral_index(float si);

        /**
         * @brief get the spectral index to emulate
         */
        float spectral_index() const;

        /**
         * @brief return the phase model that has benn specified
         */
        PhaseModelType const& phase_model() const;

        /**
         * @brief configuration for the simple phase model
         */
        SimplePhaseModelConfig const& simple_phase_model_config() const;

        /**
         * @brief set the phase model to single type with the speocife settings
         */
        void set_phase_model(SimplePhaseModelConfig const&);
        void set_phase_model(Tempo2PhaseModelConfig const&);

        /**
         * @brief the dedisperion measure to emulate
         */
        data::DedispersionMeasureType<double> dm() const;

        /**
         * @brief set the dedisperion measure to emulate
         */
        void dm(data::DedispersionMeasureType<double>);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;
        void phase_model(std::string const&);

    private:
        float _signal_to_noise;
        float _spectral_index;
        PulsarProfileId _profile_name;
        mutable PhaseModelFactory _phase_model_factory;
        data::DedispersionMeasureType<double> _dm;
        PhaseModelType _phase_model;
        SimplePhaseModelConfig _simple_phase_model_config;
        Tempo2PhaseModelConfig _tempo2_phase_model_config;
};

} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_PULSARINJECTIONCONFIG_H
