/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#include "cheetah/modules/rfim/iqrmcpu/test/IqrmImplTest.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {
namespace test {


LaggedDiffTest::LaggedDiffTest()
    : impl(iqrmcpu::Config())
    , data({10, 11, 12})
    , diff({0, 0, 0})
{

}

LaggedDiffTest::~LaggedDiffTest()
{

}

LaggedDiffTest::Array LaggedDiffTest::expected_diff(int lag) const
{
    switch (lag)
    {
        case 0: return Array({0, 0, 0});
        case 1: return Array({0, 1, 1});
        case 2: return Array({0, 1, 2});
        case -1: return Array({-1, -1, 0});
        case -2: return Array({-2, -1, 0});
    }
    if (lag >= size)
        return Array({0, 1, 2});
    else
        return Array({-2, -1, 0});
}

// Generate lagged diff tests
TEST_P(LaggedDiffTest, CorrectDiff)
{
    const int lag = GetParam();
    impl.lagged_diff(data.begin(), data.end(), lag, diff.begin());
    EXPECT_EQ(diff, expected_diff(lag));
}

INSTANTIATE_TEST_CASE_P(
    IqrmCpu,
    LaggedDiffTest,
    ::testing::Range(-(LaggedDiffTest::size + 1), LaggedDiffTest::size + 1)
);


OneHighOutlierTest::OneHighOutlierTest()
    : ::testing::TestWithParam<size_t>()
    , data(size, 0.0)
    , mask(size, false)
    , expected_mask(size, false)
{

}

OneHighOutlierTest::~OneHighOutlierTest()
{

}

void OneHighOutlierTest::SetUp()
{
    const size_t index = GetParam();
    std::fill(data.begin(), data.end(), 0.0);
    data.at(index) = 1.0;
    std::fill(expected_mask.begin(), expected_mask.end(), false);
    expected_mask.at(index) = true;
}

void OneHighOutlierTest::TearDown()
{

}

// Generate single high outlier tests
TEST_P(OneHighOutlierTest, CorrectMask)
{
    iqrmcpu::Config config;
    config.radius(2);
    iqrmcpu::IqrmImpl impl(config);
    impl.iqrm_mask(data.begin(), data.end(), mask.begin());
    EXPECT_EQ(mask, expected_mask);
}

INSTANTIATE_TEST_CASE_P(
    IqrmCpu,
    OneHighOutlierTest,
    ::testing::Range(0UL, OneHighOutlierTest::size)
);


EdgeChannelsTest::EdgeChannelsTest()
    : data(EdgeChannelsTest::size, 0)
    , mask(EdgeChannelsTest::size, false)
    , expected_mask(EdgeChannelsTest::size, false)
{

}

EdgeChannelsTest::~EdgeChannelsTest()
{

}

void EdgeChannelsTest::SetUp()
{
    const size_t size = EdgeChannelsTest::size;
    size_t ec = GetParam();
    ec = std::min(ec, size);
    std::fill(mask.begin(), mask.end(), false);
    std::fill(expected_mask.begin(), expected_mask.begin() + ec, true);
    std::fill(expected_mask.end() - ec, expected_mask.end(), true);
}

void EdgeChannelsTest::TearDown()
{

}

// Generate edge_channels tests
TEST_P(EdgeChannelsTest, CorrectMask)
{
    const size_t ec = GetParam();
    iqrmcpu::Config config;
    config.radius(1);
    config.threshold(3.0);
    config.edge_channels(ec);
    iqrmcpu::IqrmImpl impl(config);
    impl.iqrm_mask(data.begin(), data.end(), mask.begin());
    EXPECT_EQ(mask, expected_mask);
}

INSTANTIATE_TEST_CASE_P(
    IqrmCpu,
    EdgeChannelsTest,
    ::testing::Range(0UL, 2 * EdgeChannelsTest::size) // test some out of range values
);

// Generate robust_stat tests. The data have been randomly drawn. One even-length and one
// odd-length test case. The expected result is that obtained with an identical python
// implementation based on numpy.percentile().
TEST(RobustStats, TestData1)
{
    std::array<double, 4> data = {0.045, -1.141,  1.282, -0.828};
    iqrmcpu::IqrmImpl iqrm{iqrmcpu::Config()};
    iqrmcpu::IqrmImpl::RobustStats stats = iqrm.robust_stats(data.begin(), data.end());
    EXPECT_DOUBLE_EQ(stats.median, -0.3915);
    EXPECT_DOUBLE_EQ(stats.std, 0.9343958487768718);
}

TEST(RobustStats, TestData2)
{
    std::array<double, 5> data = {1.339, -0.299, -0.099, -0.131, -0.094};
    iqrmcpu::IqrmImpl iqrm{iqrmcpu::Config()};
    iqrmcpu::IqrmImpl::RobustStats stats = iqrm.robust_stats(data.begin(), data.end());
    EXPECT_DOUBLE_EQ(stats.median, -0.099);
    EXPECT_DOUBLE_EQ(stats.std, 0.027427724240177913);
}


// Test on a real 1024-channel spectral standard deviation taken with MeerTRAP at L-Band
// IQRM parameters are: radius = 100, threshold = 3.0, edge_channels = 0
// The expected output is that of the reference IQRM Python implementation at version 0.1.0, i.e.:
// https://github.com/v-morello/iqrm/tree/9f90e43c003af5d248d08b65d52fc88a73e12bbd
TEST(IqrmMask, MeerTRAPL1K)
{
    const std::vector<double> data = {
         0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,
         0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,
         0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,   0.  ,
         0.  ,   0.  ,   7.05,   6.91,   6.94,   6.88,   6.96,   6.97,   7.03,   6.96,
         6.96,   7.02,   7.03,   6.91,   6.93,   6.92,   6.94,   6.95,   7.04,   6.95,
         6.96,   6.93,   6.97,   7.15,   7.02,   6.96,   7.1 ,   6.96,   6.95,   6.98,
         6.92,   6.95,   6.99,   7.01,   6.92,   6.97,   7.02,   6.97,   6.95,   6.98,
         7.02,   7.04,   7.  ,   6.93,   7.02,   7.03,   6.99,   6.97,   7.02,   7.02,
         7.04,   7.03,   6.99,   6.94,   6.98,   7.04,   6.96,   6.95,   6.94,   7.  ,
         6.97,   6.98,   6.98,   6.99,   6.94,  35.38,  48.81,  28.7 ,  54.91,  44.56,
        45.62,  78.44,   7.03,  78.56, 119.25,  11.98,   7.05,   6.99,  33.84,   7.17,
         7.02,   7.98,   6.95,   9.08,   8.12,   7.  ,   6.95,   6.94,   7.02,   7.64,
         6.86,   8.16,   6.97,   7.64,   7.68,   7.03,   7.05,   7.04,   7.03,   6.97,
         7.02,   6.98,   6.92,   6.98,   7.07,   7.  ,   6.95,   7.03,   6.96,   6.98,
         7.02,   6.95,   7.02,   7.06,   6.93,   6.99,   7.05,   6.93,   7.  ,   7.04,
         6.98,   6.96,   6.99,   6.95,   6.96,   6.94,   6.97,   6.96,   6.95,   6.95,
         7.02,   6.96,   6.99,   7.07,   6.97,   6.98,   6.92,   7.04,   6.97,   6.94,
         7.  ,   6.96,   6.98,   7.  ,   7.02,   7.  ,   6.96,   6.98,   7.02,   6.91,
         6.98,   7.04,   6.98,   6.94,   7.  ,   6.92,   7.07,   6.97,   6.93,   6.97,
         7.  ,   7.05,   7.02,   7.  ,   6.95,   7.02,   6.98,   6.98,   6.98,   6.96,
         6.99,   6.96,   7.  ,   6.99,   6.99,   7.04,   7.2 ,   7.35,   9.31,   7.86,
         7.05,   7.23,   7.  ,   6.95,   6.95,   6.95,   6.96,   6.98,   7.02,   7.02,
         7.04,   7.48,   7.18,   6.97,   7.03,   7.02,   7.  ,   7.03,   6.94,   7.04,
         6.93,   8.19,   6.98,   6.95,   7.1 ,   7.  ,   7.07,   7.02,   7.04,   7.02,
         6.95,   7.03,   7.03,   7.09,   7.08,   6.98,   7.06,   6.99,   7.05,   7.01,
         6.98,   6.99,   6.99,   7.  ,   7.05,   6.91,   7.04,   6.99,   6.99,   7.  ,
         6.97,   7.05,   7.05,   7.04,   7.12,   6.98,   7.12,   7.26,   7.14,   7.4 ,
         7.13,   7.27,   7.64,   7.08,   8.72,   7.41,  10.31,  10.18,  12.34,  17.3 ,
        20.3 ,  16.98,  12.33,   9.43,   9.95,   8.03,   8.71,   7.57,   7.14,   7.03,
         7.16,   6.95,   6.96,   7.02,   6.97,  12.86,  11.77,   6.98,   7.  ,   6.98,
        12.3 ,   9.12,   6.99,   6.94,   7.05,   7.02,   7.26,   9.92,   9.59,   6.95,
         6.92,   7.01,   6.98,   6.99,   7.04,   6.98,   7.04,   6.97,   7.  ,   9.89,
         9.41,   6.99,   7.87,   6.93,   6.95,   7.  ,   6.99,   6.92,   6.99,   7.  ,
         6.93,   6.93,   7.05,   6.96,   7.14,   8.61,   7.05,   7.03,   6.99,   6.99,
         7.06,   7.26,   7.15,   7.42,   7.35,   7.74,   8.29,   9.23,  12.59,  12.47,
        16.3 ,  13.73,  11.45,  12.27,  10.12,   9.27,   8.14,   7.36,   7.04,   7.06,
         7.04,   7.15,   7.34,   7.33,   7.34,   7.48,   7.56,   7.31,   7.18,   7.21,
         7.01,   6.97,   6.99,   7.49,   9.27,   9.08,  11.79,  10.84,  17.78,  18.41,
        26.72,  22.72,  37.53,  16.34,  19.23,  15.21,  22.89,  10.49,  16.53,  19.53,
        15.37,  12.3 ,   8.  ,   8.95,   7.19,   6.99,   6.92,   7.05,   7.26,   7.39,
         7.86,   8.45,   8.37,   8.09,   8.1 ,   8.84,   7.2 ,   7.74,   7.3 ,   8.17,
         8.15,   8.63,  14.6 ,  13.59,  21.88,  19.92,  13.28,  21.  ,  11.61,   9.12,
        20.28,  14.34,  12.62,  12.48,  14.81,  10.69,   7.38,  15.35,   7.75,   7.38,
         7.15,   6.91,   7.02,   7.18,   6.99,   7.2 ,   7.  ,   7.46,   7.32,   7.04,
         7.19,   8.8 ,   6.84,   6.85,   7.45,   8.28,   8.11,   7.22,   7.12,   7.38,
         7.39,   7.15,   6.92,   7.34,   6.98,   6.96,   6.89,   6.89,   6.85,   6.98,
         7.02,   6.97,   7.01,   7.04,   7.17,   7.07,   7.44,   9.67,  14.98,   8.71,
         7.69,   7.02,   7.02,   7.17,   7.52,   7.2 ,   7.5 ,   7.04,   7.04,   7.09,
         6.97,   6.96,   6.94,   7.  ,   6.96,   7.14,  23.88,  21.06,  20.41,  22.41,
        23.48,  13.93,  24.22,  11.38,  11.63,  10.44,  19.14,  10.84,  19.39,  16.34,
        17.34,  10.07,  10.53,   8.53,  10.39,  11.3 ,  10.47,  10.9 ,   8.56,   8.05,
         7.27,  10.1 ,   7.31,   7.91,   7.88,   9.33,  11.38,  10.18,  21.09,   8.02,
        10.42,  11.2 ,   7.58,   7.48,   7.03,   7.21,   8.24,   9.42,   8.59,   7.3 ,
         7.  ,   7.07,   7.16,   7.1 ,   7.02,   7.03,   7.05,   6.98,   6.98,   6.95,
         7.02,   6.98,   7.03,   7.02,   7.09,   6.97,   6.95,   6.96,   6.97,   6.96,
         7.04,   6.98,   6.98,   7.  ,   6.96,   7.04,   6.98,   7.  ,   7.07,   7.  ,
         6.97,   6.93,   7.01,   7.  ,   6.97,   6.95,   7.  ,   7.04,   7.04,   6.98,
         6.98,   6.96,   7.05,   7.03,   7.06,   6.97,   7.  ,   6.98,   6.96,   6.94,
         6.92,   7.  ,   6.99,   6.95,   7.  ,   7.02,   7.01,   7.  ,   7.08,   7.02,
         7.04,   7.01,   6.94,   7.02,   6.98,   6.98,   6.94,   7.  ,   7.03,   7.02,
         7.  ,   7.02,   6.99,   6.97,   6.97,   7.  ,   7.02,   7.01,   7.01,   6.98,
         6.99,   6.95,   6.93,   7.03,   6.95,   6.97,   7.  ,   6.99,   7.02,   6.98,
         6.97,   6.99,   6.96,   6.95,   6.96,   7.  ,   6.97,   6.95,   7.03,   6.97,
         7.02,   7.02,   7.  ,   6.99,   7.  ,   7.06,   6.99,   6.97,   6.98,   6.98,
         6.96,   7.01,   7.  ,   7.05,   7.03,   6.98,   7.04,   7.04,   6.97,   7.02,
         6.96,   6.98,   6.98,   6.98,   7.05,   6.99,   7.04,   6.97,   7.06,   7.01,
         7.01,   7.07,   7.04,   6.95,   7.  ,   7.  ,   6.98,   6.99,   6.99,   7.03,
         7.02,   6.97,   7.06,   7.  ,   6.97,   7.06,   6.95,   6.96,   7.03,   6.94,
         6.97,   7.01,   7.02,   7.01,   6.96,   7.04,   7.  ,   6.98,   6.98,   7.02,
         7.01,   7.01,   6.96,   6.96,   7.  ,   7.01,   7.05,   6.99,   6.99,   7.03,
         7.07,   7.01,   6.99,   6.99,   7.04,   6.98,   7.02,   6.99,   7.06,   6.99,
         7.02,   7.  ,   6.94,   7.  ,   6.94,   6.99,   7.02,   7.08,   7.04,   7.05,
         7.04,   7.04,   6.98,   7.06,   6.98,   6.99,   6.96,   7.02,   7.  ,   6.95,
         7.  ,   7.04,   7.03,   7.06,   7.07,   7.04,   7.  ,   6.98,   6.99,   6.96,
         6.96,   6.95,   6.97,   6.97,   7.02,   6.98,   6.98,   6.99,   7.06,   7.05,
         7.  ,   6.99,   7.02,   7.05,   6.99,   7.  ,   7.03,   7.11,   6.97,   7.01,
         6.96,   7.05,   6.98,   6.96,   6.96,   7.05,   6.96,   7.  ,   7.01,   7.  ,
         7.  ,   6.99,   7.04,   7.04,   7.04,   7.  ,   7.05,   6.93,   7.04,   6.94,
         7.03,   6.98,   6.94,   6.95,   7.04,   7.04,   7.03,   6.97,   6.98,   7.08,
         7.01,   6.94,   7.  ,   6.95,   7.04,   7.01,   6.95,   6.97,   6.96,   6.96,
         6.96,   6.88,   6.71,  12.85,  13.12,  10.92,   8.93,  22.27,   6.96,  15.8 ,
         7.43,  11.59,  21.48,  38.44,   7.88,  13.94,  12.73,  23.48,   9.82,  13.71,
         8.66,  22.14,   7.91,  22.81,   9.25,  34.56,   6.97,   7.04,   6.99,   9.4 ,
         7.14,  23.3 ,   7.85,  10.2 ,   7.3 ,   7.68,   7.07,   9.32,   8.78,   7.5 ,
        17.11,  53.19,  56.78,  44.16,  23.55,   8.3 ,   7.26,   8.44,   7.23,   7.23,
         7.04,   7.02,   7.15,   9.3 ,   9.7 ,   7.64,   7.32,  15.69,   7.16,  34.09,
        53.38,  36.94,  40.84,   6.93,   7.63,   7.55,   7.88,   8.19,  13.55,   7.71,
         7.06,   6.96,   7.36,   7.  ,  12.43,   8.09,  11.04,  21.41,  13.99,  11.29,
        10.78,   8.75,   7.21,   8.73,   7.79,   7.02,   7.23,   7.11,   6.89,   7.14,
         7.3 ,   7.3 ,   7.01,   9.16,   8.78,  24.7 ,   9.61,   7.93,   7.29,   7.14,
         7.06,   6.97,   7.03,   7.06,   6.98,   6.92,   7.  ,   7.05,   7.02,   6.94,
         6.98,   7.05,   6.95,   6.98,   6.99,  12.6 ,  25.41,  19.34,  29.75,  38.91,
        26.88,  22.59,   9.84,   7.04,   6.93,   7.02,   7.03,   7.  ,   6.98,   7.07,
         6.98,   7.  ,   7.06,   6.93,   6.98,   6.96,   6.89,   6.96,   7.02,   6.95,
         7.01,   7.04,   6.97,   7.03,   6.92,   6.98,   6.97,   7.04,   7.06,   6.96,
         6.93,   6.95,   7.01,   6.98,   6.96,   7.04,   7.02,   6.94,   7.01,   6.95,
         7.04,   7.02,   7.03,   6.97,   6.95,   6.93,   6.97,   7.02,   6.98,   7.01,
         6.96,   7.  ,   7.03,   6.93,   7.03,   7.  ,   6.99,   6.96,   7.02,   6.91,
         7.  ,   7.04,   7.04,   6.95,   6.98,   7.12,   7.06,   7.12,   7.03,   7.09,
         7.47,   6.99,   6.95,   6.95,   6.95,   8.03,   6.99,   7.01,   6.95,   6.96,
         6.96,   7.03,   6.98,   6.97,   7.02,   6.88,   6.91,   6.98,   6.97,   7.02,
         6.94,   6.97,   7.01,   6.95,   7.04,   7.03,   6.96,   7.01,   6.97,   7.  ,
         6.96,   6.98,   7.01,   7.47
    }; // data

    const std::vector<bool> expected_mask = {
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false,  true,  true,  true,  true,  true,  true,  true,  true,
        true, false, false,  true,  true,  true, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false,  true,  true,  true,  true,  true,
        true,  true, false,  true,  true,  true, false, false,  true, false,
       false,  true, false,  true,  true, false, false, false, false,  true,
       false,  true, false,  true,  true, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false,  true,  true,  true,
       false, false, false, false, false, false, false, false, false, false,
       false,  true, false, false, false, false, false, false, false, false,
       false,  true, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false,  true,
       false, false,  true, false,  true, false,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true,  true, false, false,
       false, false, false, false, false,  true,  true, false, false, false,
        true,  true, false, false, false, false, false,  true,  true, false,
       false, false, false, false, false, false, false, false, false,  true,
        true, false,  true, false, false, false, false, false, false, false,
       false, false, false, false, false,  true, false, false, false, false,
       false, false, false,  true, false,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false,  true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
        true,  true, false,  true, false, false, false, false, false, false,
       false,  true,  true, false, false,  true, false, false, false,  true,
        true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true, false,  true,  true, false,
       false, false, false, false, false, false, false, false, false, false,
       false,  true, false, false, false,  true,  true, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false,  true,  true,  true,  true,
        true, false, false, false,  true, false,  true, false, false, false,
       false, false, false, false, false, false,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
       false,  true, false,  true,  true,  true,  true,  true,  true,  true,
        true,  true,  true, false, false, false,  true,  true,  true, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false,  true,  true,  true,  true,  true, false,  true,
        true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true, false, false, false,  true,
       false,  true,  true,  true, false,  true, false,  true,  true, false,
        true,  true,  true,  true,  true,  true, false,  true, false, false,
       false, false, false,  true,  true, false, false,  true, false,  true,
        true,  true,  true, false, false, false, false,  true,  true, false,
       false, false, false, false,  true,  true,  true,  true,  true,  true,
        true,  true, false,  true,  true, false, false, false, false, false,
       false, false, false,  true,  true,  true,  true,  true, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false,  true,  true,  true,  true,  true,
        true,  true,  true, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
        true, false, false, false, false,  true, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false, false, false,
       false, false, false,  true
    }; // expected_mask

    iqrmcpu::Config config;
    config.radius(100);
    config.threshold(3.0);
    config.edge_channels(0);
    iqrmcpu::IqrmImpl impl(config);

    std::vector<bool> mask(data.size(), false);
    impl.iqrm_mask(data.begin(), data.end(), mask.begin());
    EXPECT_EQ(mask, expected_mask);
}

} // namespace test
} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
