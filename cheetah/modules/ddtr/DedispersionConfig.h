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
#ifndef SKA_CHEETAH_MODULES_DDTR_DEDISPERSIONCONFIG_H
#define SKA_CHEETAH_MODULES_DDTR_DEDISPERSIONCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief
 *   describes dedispersion configuration
 *
 */

class DedispersionConfig : public utils::Config
{
    public:
        typedef data::DedispersionMeasureType<float> Dm;

    public:
        DedispersionConfig();
        DedispersionConfig(Dm start, Dm stop, Dm step);
        ~DedispersionConfig();

        /**
         * @brief the first dm condidate to try
         */
        Dm dm_start() const;
        void dm_start(Dm);

        /**
         * @brief the last dm condidate to try
         */
        Dm dm_end() const;
        void dm_end(Dm);

        /**
         * @brief the step size to take in generating dm trials between start and end
         */
        Dm dm_step() const;
        void dm_step(Dm);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        Dm _dm_start;
        Dm _dm_end;
        Dm _dm_step;
};


} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_DEDISPERSIONCONFIG_H
