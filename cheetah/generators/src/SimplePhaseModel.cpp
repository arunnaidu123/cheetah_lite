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
#include "cheetah/generators/src/SimplePhaseModel.h"
#include "cheetah/generators/SimplePhaseModelConfig.h"

namespace ska {
namespace cheetah {
namespace generators {


SimplePhaseModel::SimplePhaseModel(SimplePhaseModelConfig const& config)
    : _config(config)
{
}

SimplePhaseModel::~SimplePhaseModel()
{
}

double SimplePhaseModel::operator()(utils::ModifiedJulianClock::time_point const& mjd, boost::units::quantity<data::MegaHertz, double> const ) const
{
    auto const& coeff = _config.coefficients();
    double delta = (mjd - _epoch).count();
    double x = delta;

    double total=0.0;
    for(auto it=coeff.begin(); it < coeff.end(); ++it) {
        total += x * *it;
        x = x * delta;
    }
    return total;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
