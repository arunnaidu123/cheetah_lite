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
#ifndef SKA_CHEETAH_MODULES_RFIM_METRICS_H
#define SKA_CHEETAH_MODULES_RFIM_METRICS_H
#include "cheetah/data/TimeFrequencyFlags.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief
 *      A class for analysing and storing the results of the difference between two sets of flags representing RFI
 *      detection.
 */

class Metrics
{
    public:
        typedef data::TimeFrequencyFlags<> TimeFrequencyFlagsType;

    public:
        Metrics(TimeFrequencyFlagsType const& expected, TimeFrequencyFlagsType const& actual);
        ~Metrics();

        /**
         * @brief calculate the metrics by comparing the two sets of flags
         */
        std::size_t compare(TimeFrequencyFlagsType const& expected, TimeFrequencyFlagsType const& actual);

        /**
         * @brief return the total of any correctly identified  flags found
         */
        std::size_t num_correct() const;

        /**
         * @brief return the number of rfi flags in the expected data
         */
        std::size_t num_rfi() const;

        /**
         * @brief return the total of any correct flags as a percentage
         */
        float rfi_detected_percentage() const;

        /**
         * @brief return the total of any correct flags as a percentage
         */
        float correct_percentage() const;

        /**
         * @brief returnn a set of flags indicating each false positive fopund
         */
        TimeFrequencyFlagsType const& false_positives() const;

        /**
         * @brief return the total of any false positives detected
         */
        std::size_t num_false_positives() const;
        float false_positives_percentage() const;

        /**
         * @brief returnn a set of flags indicating each false negative found
         */
        TimeFrequencyFlagsType const& false_negatives() const;

        /**
         * @brief return the number of any false negatives detected
         */
        std::size_t num_false_negatives() const;
        float false_negatives_percentage() const;

    private:
        TimeFrequencyFlagsType _false_positives;
        TimeFrequencyFlagsType _false_negatives;
        std::size_t _false_positives_num;
        std::size_t _false_negatives_num;
        std::size_t _correct_num;
        std::size_t _rfi_num;
};


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_METRICS_H
