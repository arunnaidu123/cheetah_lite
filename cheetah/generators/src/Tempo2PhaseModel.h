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
#ifndef SKA_CHEETAH_GENERATORS_TEMPO2PHASEMODEL_H
#define SKA_CHEETAH_GENERATORS_TEMPO2PHASEMODEL_H

#include "cheetah/data/Units.h"
#include "cheetah/generators/Tempo2PhaseModelConfig.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include <memory>

//namespace {
//}

namespace ska {
namespace cheetah {
namespace generators {

struct T2Predictor;

/**
 * @brief
 *    Parameters for specifying the tempo2 phase model
 * @details
 */

class Tempo2PhaseModel
{
    public:
        Tempo2PhaseModel(Tempo2PhaseModelConfig const&);
        ~Tempo2PhaseModel();

        double operator()(utils::ModifiedJulianClock::time_point const& mjd, boost::units::quantity<data::MegaHertz, double> const mhz) const;

        /**
         * @brief load in model data from a tempo2 predictor file
         */
        void load(boost::filesystem::path const& tempo2_predictor_file);

    private:
        std::shared_ptr<T2Predictor> _pred;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_TEMPO2PHASEMODEL_H
