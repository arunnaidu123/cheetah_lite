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
#include "cheetah/utils/test/ConvolvePlanTest.h"
#include "cheetah/utils/ConvolvePlan.h"
#include <array>
#include <algorithm>

namespace ska {
namespace cheetah {
namespace utils {
namespace test {


ConvolvePlanTest::ConvolvePlanTest()
    : ::testing::Test()
{
}

ConvolvePlanTest::~ConvolvePlanTest()
{
}

TEST_F(ConvolvePlanTest, test_all_null_function)
{
    constexpr std::size_t npts=100;
    std::array<float, npts> a;
    std::fill(a.begin(), a.end(), 0.0);
    std::array<float, npts> b;
    std::fill(b.begin(), b.end(), 0.0);
    std::array<float, npts+1> output;
    std::fill(output.begin(), output.end(), 0.0);
    
    utils::ConvolvePlan plan(npts, a.data(), b.data(), output.data());
    plan.convolve();
    std::for_each(output.begin(), output.end(), [](float t) { ASSERT_EQ(0.0, t); });
}

TEST_F(ConvolvePlanTest, test_a_null_function)
{
    constexpr std::size_t npts=100;
    std::array<float, npts> a;
    std::fill(a.begin(), a.end(), 0.0);
    std::array<float, npts> b;
    std::fill(b.begin(), b.end(), 1.0);
    std::array<float, npts+1> output;
    std::fill(output.begin(), output.end(), 0.0);
    
    utils::ConvolvePlan plan(npts, a.data(), b.data(), output.data());
    plan.convolve();
    std::for_each(output.begin(), output.end(), [](float t) { ASSERT_EQ(0.0, t); });
}

TEST_F(ConvolvePlanTest, test_b_null_function)
{
    constexpr std::size_t npts=100;
    std::array<float, npts> a;
    std::fill(a.begin(), a.end(), 1.0);
    std::array<float, npts> b;
    std::fill(b.begin(), b.end(), 0.0);
    std::array<float, npts+1> output;
    std::fill(output.begin(), output.end(), 0.0);
    
    utils::ConvolvePlan plan(npts, a.data(), b.data(), output.data());
    plan.convolve();
    std::for_each(output.begin(), output.end(), [](float t) { ASSERT_EQ(0.0, t); });
}

TEST_F(ConvolvePlanTest, test_a_equal_b)
{
    constexpr std::size_t npts=100;
    std::array<float, npts> a;
    std::fill(a.begin(), a.end(), 1.0);
    std::array<float, npts> b;
    std::fill(b.begin(), b.end(), 1.0);
    std::array<float, npts+1> output;
    std::fill(output.begin(), output.end(), 0.0);
    
    utils::ConvolvePlan plan(npts, a.data(), b.data(), output.data());
    plan.convolve();
    ASSERT_EQ(0.0, output.back()); // check no buffer overwrite
    std::for_each(output.begin(), output.end() - 1, [&](float t) { ASSERT_EQ(100.0, t); });
}

} // namespace test
} // namespace utils
} // namespace cheetah
} // namespace ska
