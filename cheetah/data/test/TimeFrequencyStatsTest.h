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
#ifndef SKA_CHEETAH_DATA_TEST_TIMEFREQUENCYSTATSTEST_H
#define SKA_CHEETAH_DATA_TEST_TIMEFREQUENCYSTATSTEST_H

#include "cheetah/data/TimeFrequencyStats.h"
#include <gtest/gtest.h>
#include <tuple>
#include <limits>


namespace ska {
namespace cheetah {
namespace data {
namespace test {

/**
 * @brief Helper struct that serves as template parameter to TimeFrequencyStatsTest
 * It contains the numerical type for the test data block to generate, and the values
 * that the test data shall contain (see TimeFrequencyStats for details)
 */
template <typename _T, int _A, int _B>
struct TimeFrequencyStatsTestParams
{
    typedef _T TF_Data;
    typedef typename TF_Data::NumericalRep  NumericalT;
    static constexpr int a = _A;
    static constexpr int b = _B;

    static_assert(a >= std::numeric_limits<NumericalT>::lowest(), "A is below limits of T");
    static_assert(a <= std::numeric_limits<NumericalT>::max()   , "A is above limits of T");
    static_assert(b >= std::numeric_limits<NumericalT>::lowest(), "B is below limits of T");
    static_assert(b <= std::numeric_limits<NumericalT>::max()   , "B is above limits of T");
};

/**
 * @brief Test TimeFrequencyStats calculation and accuracy on a "chequerboard" data block with
 *   different numerical data types.
 * @details Generate TF data like this:
 *     <-- Frequency -->
 *     A B A B A B ...
 *     B A B A B A ...
 *     A B A B A B ...
 *     ...
 * where A and B are some fixed values.
 * The mean and stddev can be calculated analytically. For every channel and every spectrum, they are:
 * mean = (A + B) / 2
 * stddev = |A - B| / 2
 * NOTE: The stddev value above is *without* Bessel's correction
 */
template <typename TestParamsType>
class TimeFrequencyStatsTest : public ::testing::Test
{
    public:
        TimeFrequencyStatsTest();
        ~TimeFrequencyStatsTest();

    protected:
        //typedef typename TestParamsType::NumericalT NumericalT;
        typedef typename TestParamsType::TF_Data TFData;
        //typedef TimeFrequency<Cpu, NumericalT> TFData;
        typedef TimeFrequencyStats<TFData> TFStats;

        /**
         * Test data block dimensions
         * NOTE: make one dimension large, check if this causes inaccuracy issues
         * IMPORTANT: Both dimensions MUST be even numbers, the formulas to calculate
         * the expected values for the statistics depend on this assumption
         **/
        static constexpr size_t nchan = 8;
        static constexpr size_t nsamp = 262144;

        // Relative tolerance on all stat values
        static constexpr float relative_tolerance = 5.0e-3;

        // Values of the "chequerboard" pattern (see above)
        static constexpr float a = TestParamsType::a;
        static constexpr float b = TestParamsType::b;

    protected:
        void SetUp() override;
        void TearDown() override;

        float expected_mean() const;
        float expected_variance() const;
        float expected_stddev() const;

    protected:
        TFData _data;
};


} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_TEST_TIMEFREQUENCYSTATSTEST_H
