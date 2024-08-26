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
#ifndef SKA_CHEETAH_MODULES_RFIM_IQRMCPU_TEST_IQRMIMPLTEST_H
#define SKA_CHEETAH_MODULES_RFIM_IQRMCPU_TEST_IQRMIMPLTEST_H

#include "cheetah/modules/rfim/iqrmcpu/detail/IqrmImpl.h"
#include <gtest/gtest.h>
#include <array>
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {
namespace test {

/**
 * @brief Test IqrmImpl::lagged_diff on fixed input data for a few lag values
 */
class LaggedDiffTest : public ::testing::TestWithParam<int>
{
    public:
        static constexpr int size = 3; // Size of the input test data

    public:
        LaggedDiffTest();
        ~LaggedDiffTest();

    protected:
        typedef typename std::array<int, size> Array;

    protected:
        Array expected_diff(int lag) const;

    protected:
        const iqrmcpu::IqrmImpl impl;
        const Array data;
        Array diff;
};

/**
 * @brief Test IQRM on fake spectral stat input that contains a single high outlier.
 *
 * @details
 * In this test case, the input spectral stat has a fixed length, and all the values are 0 except
 * for one element whose value is 1 (the high outlier).
 * The IQRM radius parameter is set to 2; due to how the algorithm is designed, an outlier placed
 * at either edge of the input data won't get flagged if the radius is 1 (it would not have enough
 * "votes" against it).
 * The parameter of the test is the index of the high outlier.
 * The expected result is that only the high outlier is flagged.
 */
class OneHighOutlierTest : public ::testing::TestWithParam<size_t>
{
    public:
        static constexpr size_t size = 8; // Size of the input test data

    public:
        OneHighOutlierTest();
        ~OneHighOutlierTest();

    protected:
        typedef typename std::vector<float> Data;
        typedef typename std::vector<bool> Mask;

    protected:
        void SetUp() override;
        void TearDown() override;

    protected:
        Data data;
        Mask mask;
        Mask expected_mask;
};

/**
 * @brief Check that the edge_channels parameter behaves properly on data filled with zeros. The
 *  parameter of the test is edge_channels.
 */
class EdgeChannelsTest : public ::testing::TestWithParam<size_t>
{
    public:
        static constexpr size_t size = 8; // Size of the input test data

    public:
        EdgeChannelsTest();
        ~EdgeChannelsTest();

    protected:
        typedef typename std::vector<float> Data;
        typedef typename std::vector<bool> Mask;

    protected:
        void SetUp() override;
        void TearDown() override;

    protected:
        const Data data;
        Mask mask;
        Mask expected_mask;
};

} // namespace test
} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_IQRMCPU_TEST_IQRMIMPLTEST_H