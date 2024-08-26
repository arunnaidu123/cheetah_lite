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
#ifndef SKA_CHEETAH_GENERATORS_SIMPLEPHASEMODEL_H
#define SKA_CHEETAH_GENERATORS_SIMPLEPHASEMODEL_H

#include "cheetah/data/Units.h"
#include "cheetah/utils/ModifiedJulianClock.h"

namespace ska {
namespace cheetah {
namespace generators {
class SimplePhaseModelConfig;

/**
 * @brief
 *    A simple Taylor Expansion phase model 
 * @details
 *    Swt the coefficents and the reference epoch in the config object.
 *
 *    phi(t) = phi(t_0) + nu * (t - t_0) + nu_dot * (t - t_0)^2 ...
 */

class SimplePhaseModel
{
    public:
        SimplePhaseModel(SimplePhaseModelConfig const&);
        ~SimplePhaseModel();

        double operator()(utils::ModifiedJulianClock::time_point const& mjd, boost::units::quantity<data::MegaHertz, double> const mhz) const;

    private:
        SimplePhaseModelConfig const& _config;
        utils::ModifiedJulianClock::time_point _epoch;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_SIMPLEPHASEMODEL_H
