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
#ifndef SKA_CHEETAH_GENERATORS_DISPERSEDPULSE_H
#define SKA_CHEETAH_GENERATORS_DISPERSEDPULSE_H


#include "DataGenerator.h"
#include "DispersedPulseConfig.h"
#include "pss/astrotypes/units/DispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief Inject a single pulse at a specified dispersion measure
 * @details
 */

template<typename DataType>
class DispersedPulse : public DataGenerator<DataType>
{
        typedef DataGenerator<DataType> BaseT;
        typedef typename DataType::TimeType TimeType;
        typedef typename DataType::NumericalRep NumericalRep;

    public:
        DispersedPulse();
        DispersedPulse(DispersedPulseConfig const&);
        ~DispersedPulse();

        void next(DataType& data) override;

    private:
        typedef double DispersionConstant; // TODO make boost::units::quantity in s_mhz_squared_cm_cubed_per_pc
        DispersionConstant _dm_constant;
        pss::astrotypes::units::DispersionMeasure<double> const _dm_measure;
        TimeType _pulse_width;
        NumericalRep _delta;    // amount to increase signal by for pulse
};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/DispersedPulse.cpp"

#endif // SKA_CHEETAH_GENERATORS_DISPERSEDPULSE_H
