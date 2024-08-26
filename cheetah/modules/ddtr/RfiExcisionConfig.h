/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_DDTR_RFIEXCISIONCONFIG_H
#define SKA_CHEETAH_MODULES_DDTR_RFIEXCISIONCONFIG_H


#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief Configuration for the paramters for flagging RFI during copy to the Aggregate buffer
 * two main parameters:  ideal_rms (rms of the cleanest part of the data)
 * threshold (threshold to use (in multiples of rms) to flag data while copying)
 * active: boolean to activate the thresholding
 * @details
 */

class RfiExcisionConfig:  public cheetah::utils::Config
{
    public:
        RfiExcisionConfig();

        ~RfiExcisionConfig();

        float ideal_rms() const;

        float threshold() const;

        bool active() const;


    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        float _ideal_rms;
        float _threshold;
        bool _active;
};


} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_RFIEXCISIONCONFIG_H
