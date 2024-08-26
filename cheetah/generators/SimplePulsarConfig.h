/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2024 The SKA organisation
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
#ifndef SKA_CHEETAH_GENERATORS_SIMPLEPULSARCONFIG_H
#define SKA_CHEETAH_GENERATORS_SIMPLEPULSARCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *
 * @details
 *
 */

class SimplePulsarConfig : public cheetah::utils::Config
{
    public:
        typedef data::DedispersionMeasureType<double> DmType;
        typedef data::TimeType TimeType;
    public:
        SimplePulsarConfig();
        ~SimplePulsarConfig();

        /**
        * @brief getter and setter for mean.
        */
        float mean() const;
        void mean(float);

        /**
        * @brief getter and setter for standard deviation.
        */
        float std_deviation() const;
        void std_deviation(float);

        /**
        * @brief getter and setter for dispersion measure.
        */
        DmType dm() const;
        void dm(DmType);

        /**
        * @brief getter and setter for period.
        */
        TimeType period() const;
        void period(TimeType);

        /**
        * @brief getter and setter for width.
        */
        TimeType width() const;
        void width(TimeType);

        /**
        * @brief getter and setter for Signal to noise of pulse.
        */
        float snr() const;
        void snr(float);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        float _mean;
        float _deviation;
        DmType _dm;
        TimeType _period;
        TimeType _width;
        float _snr;
};

} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_SIMPLEPULSARCONFIG_H
