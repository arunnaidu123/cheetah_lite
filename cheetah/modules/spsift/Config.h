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
#ifndef SKA_CHEETAH_MODULES_SPSIFT_CONFIG_H
#define SKA_CHEETAH_MODULES_SPSIFT_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/Units.h"
#include "pss/astrotypes/units/DispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spsift {

/**
 * @brief
 * @details
 */

class Config : public utils::Config
{
    public:
        typedef boost::units::quantity<data::MilliSeconds, double> MsecTimeType;
        typedef pss::astrotypes::units::DispersionMeasure<float> Dm;

    public:
        Config();
        ~Config();

        /**
         * @brief pulse width threshold *above* which single pulse candidates are removed
         */
        MsecTimeType pulse_width_threshold() const;
        void pulse_width_threshold(MsecTimeType const& width_threshold);

        /**
         * @brief S/N threshold *below* which single pulse candidates are removed
         */
        float sigma_threshold() const;
        void sigma_threshold(float const& sigma_threshold);

        /**
         * @brief DM threshold *below* which single pulse candidates are removed
         */
        Dm dm_threshold() const;
        void dm_threshold(Dm const& _dm_threshold);

        /**
         * @brief Maximum number of candidates that are allowed to remain in the SpCcl
         * object after the thresholding has been applied (0 = unlimited candidates).
         * If the number of candidates exceeds this limit, the SpCcl candidate list is
         * truncated.
         */
        std::size_t maximum_candidates() const;
        void maximum_candidates(std::size_t const& numcands_threshold);

        bool active() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        MsecTimeType _pulse_width_threshold;
        float _sigma_threshold;
        Dm _dm_threshold;
        std::size_t _maximum_candidates;
        bool _active;
};


} // namespace spsift
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_SPSIFT_CONFIG_H
