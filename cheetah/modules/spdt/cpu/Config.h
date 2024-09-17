/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_SPDT_CPU_CONFIG_H
#define SKA_CHEETAH_MODULES_SPDT_CPU_CONFIG_H


#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace cpu {

/**
 * @brief
 *   Configuration for the Spdt cpu
 */

class Config : public utils::Config
{
    public:
        Config();
        ~Config();

        float candidate_rate() const;
        void candidate_rate(float cands_per_second);

        /**
         * @brief returns true if algorithm is selected
         */
        bool active() const;

        /**
         * @brief activate the algorithm
         */
        void active(bool val);

        /**
         * @brief number of samples per iteration
         * @details The cpu spdt algorithm searches the time series in blocks of size defined by samples_per_iteration.
         * Each iteration will output only one candidate with max SNR in the block.
         */
        std::size_t samples_per_iteration() const;
        void samples_per_iteration(std::size_t const& value);

        /**
         * @brief total number of widths to be used for box-car matched filter
         * @details The cpu spdt algorithm uses the widths in the power of 2s.
         * For example setting number_of_widths = 4 results in width search with box-car sizes of  1, 2, 4, 8 samples.
         */
        std::size_t number_of_widths() const;
        void number_of_widths(std::size_t const& value);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        bool     _active;
        std::size_t _samples_per_iteration;
        std::size_t _number_of_widths;
};


} // namespace cpu
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_SPDT_CPU_CONFIG_H
