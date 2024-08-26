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
#include "cheetah/data/test/TimeFrequencyStatsTest.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

// Must explicitly define a and b here to avoid linker error in C++11
// See: https://stackoverflow.com/questions/40690260/undefined-reference-error-for-static-constexpr-member
template <typename TestParamsType>
constexpr float TimeFrequencyStatsTest<TestParamsType>::a;

template <typename TestParamsType>
constexpr float TimeFrequencyStatsTest<TestParamsType>::b;


template <typename TestParamsType>
TimeFrequencyStatsTest<TestParamsType>::TimeFrequencyStatsTest()
    : ::testing::Test()
{
}

template <typename TestParamsType>
TimeFrequencyStatsTest<TestParamsType>::~TimeFrequencyStatsTest()
{
}

template <typename TestParamsType>
void TimeFrequencyStatsTest<TestParamsType>::SetUp()
{
    _data = TFData{
        data::DimensionSize<data::Time>(nsamp),
        data::DimensionSize<data::Frequency>(nchan)
    };

    // Generate the chequerboard data
    for (size_t isamp = 0; isamp < nsamp; ++isamp)
    {
        typename TFData::Spectra spectrum = _data.spectrum(isamp);
        if (isamp % 2 == 0)
            for (data::DimensionIndex<data::Frequency> j(0); j < nchan - 1; j += 2)
            {
                spectrum[j] = a;
                spectrum[j+1] = b;
            }
        else
            for (data::DimensionIndex<data::Frequency> j(0); j < nchan - 1; j += 2)
            {
                spectrum[j] = b;
                spectrum[j+1] = a;
            }
    }
}

template <typename TestParamsType>
void TimeFrequencyStatsTest<TestParamsType>::TearDown()
{
}

template <typename TestParamsType>
float TimeFrequencyStatsTest<TestParamsType>::expected_mean() const
{
    return (a + b) / 2.0;
}

template <typename TestParamsType>
float TimeFrequencyStatsTest<TestParamsType>::expected_variance() const
{
    const float s = expected_stddev();
    return s * s;
}

template <typename TestParamsType>
float TimeFrequencyStatsTest<TestParamsType>::expected_stddev() const
{
    return std::abs(a - b) / 2.0;
}

TYPED_TEST_SUITE_P(TimeFrequencyStatsTest);

TYPED_TEST_P(TimeFrequencyStatsTest, test_stats_calculation)
{
    typedef typename TimeFrequencyStatsTest<TypeParam>::TFData TFData;
    typedef typename TimeFrequencyStatsTest<TypeParam>::TFStats TFStats;
    typedef typename TFStats::ReturnType ReturnType;

    const float rtol = this->relative_tolerance;

    const float mean = this->expected_mean();
    const float mean_tol = std::abs(mean * rtol);

    const float variance = this->expected_variance();
    const float variance_tol = std::abs(variance * rtol);

    const float stddev = this->expected_stddev();
    const float stddev_tol = std::abs(stddev * rtol);

    // Procedure to check channel stats and spectrum stats is the same
    const auto check_accuracy = [&](const ReturnType& stats_container) {
        for (auto const& s : stats_container)
        {
            EXPECT_NEAR(s.mean(), mean, mean_tol);
            EXPECT_NEAR(s.variance(), variance, variance_tol);
            EXPECT_NEAR(s.stddev(), stddev, stddev_tol);
        }
    };

    // Calculate stats
    TFStats stats{std::make_shared<TFData>(this->_data)};

    // Check channel stats
    const ReturnType& channel_stats = stats.channel_stats();
    EXPECT_EQ(channel_stats.size(), this->_data.number_of_channels());
    check_accuracy(channel_stats);

    // Check spectrum stats
    auto const& spectrum_stats = stats.spectrum_stats();
    EXPECT_EQ(spectrum_stats.size(), this->_data.number_of_spectra());
    check_accuracy(spectrum_stats);
}

REGISTER_TYPED_TEST_SUITE_P(TimeFrequencyStatsTest, test_stats_calculation);

/**
 * Test for all "standard" data types: uint8, uint16, float
 * Some values are chosen specifically as tests of numerical stability
 * (e.g. one small and one large value, or two values such that mean >> stddev)
 * See also: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
 */
using TestParamsTypeList = typename ::testing::Types<
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu, uint8_t>,     0,   255>,
    TimeFrequencyStatsTestParams< FrequencyTime<Cpu, uint8_t>,     0,   255>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu, uint8_t>,     1,   255>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu, uint8_t>,   255,   255>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu, uint8_t>,    64,   192>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu, uint8_t>,   107,   142>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu, uint8_t>,   254,   255>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu,uint16_t>,     1, 65535>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu,uint16_t>, 65534, 65535>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu,   float>,   -42,   142>,
    TimeFrequencyStatsTestParams< FrequencyTime<Cpu,   float>,   -42,   142>,
    TimeFrequencyStatsTestParams< TimeFrequency<Cpu,   float>,     0, 99999>,
    TimeFrequencyStatsTestParams< FrequencyTime<Cpu,   float>,     0, 99999>
>;

INSTANTIATE_TYPED_TEST_SUITE_P(
    TimeFrequencyStats,
    TimeFrequencyStatsTest,
    TestParamsTypeList
);


} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
