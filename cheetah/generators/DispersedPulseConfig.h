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
#ifndef SKA_CHEETAH_GENERATORS_DISPERSEDPULSECONFIG_H
#define SKA_CHEETAH_GENERATORS_DISPERSEDPULSECONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/Units.h"
#include "pss/astrotypes/units/DispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief Configuration parameters for the DispersedPulse generator
 */

class DispersedPulseConfig : public cheetah::utils::Config
{
        typedef cheetah::utils::Config BaseT;
        typedef boost::units::quantity<boost::units::si::time, double> TimeType;

    public:
        DispersedPulseConfig(std::string const& tag_name="dispersed_pulse");
        ~DispersedPulseConfig();

        /**
         * @brief the Dispersin Measure of the pulse
         */
        pss::astrotypes::units::DispersionMeasure<double> dispersion_measure() const;

        /**
         * @brief The pulse width (ms)
         */
        TimeType pulse_width() const;

        /// the brightness of the pulse (signal increase from background)
        double delta() const;

    protected:
        void add_options(OptionsDescriptionEasyInit&) override;

    private:
        pss::astrotypes::units::DispersionMeasure<double> _dispersion_measure;
        double _delta;    // amount to increase signal by for pulse
        TimeType _pulse_width;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_DISPERSEDPULSECONFIG_H
