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
#ifndef SKA_CHEETAH_GENERATORS_PULSARINJECTIION_H
#define SKA_CHEETAH_GENERATORS_PULSARINJECTIION_H

#include "DataGenerator.h"
#include "PulsarInjectionConfig.h"
#include "pulse_profile/PulsarProfile.h"
#include <complex>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <numeric>
#include <functional>

namespace ska {
namespace cheetah {
namespace generators {
class ProfileManager;
/**
 * @brief
 *   Add a pulsar profile into the existing data
 *
 * @details
 *   Takes a profile shape and merges it with the existing data in the
 *   TimeFrequency block scaled to match the required signal to nise ratio.
 */
template<typename DataType>
class PulsarInjection : public DataGenerator<DataType>
{
        typedef DataGenerator<DataType> BaseT;

    public:
        PulsarInjection(PulsarInjectionConfig const&, ProfileManager const& profile_manager);
        ~PulsarInjection();

        virtual void next(DataType& data) override;

    private:
        PulsarInjectionConfig const& _config;
        ProfileManager const& _profile_manager;
        PulsarProfile _profile;
        std::function<double(utils::ModifiedJulianClock::time_point const&, boost::units::quantity<data::MegaHertz, double>)> _phase_model;
        std::size_t _last_number_of_channels;
        std::size_t _last_number_of_spectra;
};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/PulsarInjection.cpp"

#endif // SKA_CHEETAH_GENERATORS_PULSARINJECTIION_H

