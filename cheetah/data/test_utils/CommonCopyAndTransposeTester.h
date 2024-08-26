/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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

#ifndef SKA_CHEETAH_CORNER_TURN_TEST_UTILS_COMMONCOPYANDTRANSPOSETESTER_H
#define SKA_CHEETAH_CORNER_TURN_TEST_UTILS_COMMONCOPYANDTRANSPOSETESTER_H

#include "cheetah/utils/test_utils/AlgorithmTester.h"
#include "cheetah/corner_turn/CornerTurn.h"
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template <typename SrcT, typename DstT>
struct CommonCopyAndTransposeTesterTraits
{
    typedef SrcT SrcType;
    typedef DstT DstType;

};

/**
 * @brief Copy and Transpose testers for TF and FT data types
 */

template <typename CommonCopyAndTransposeTesterTraitsT>
class CommonCopyAndTransposeTester :  public ::testing::TestWithParam<CommonCopyAndTransposeTesterTraitsT>
{
        typedef ::testing::TestWithParam<CommonCopyAndTransposeTesterTraitsT> BaseT;
    protected:
        void SetUp() override;
        void TearDown() override;

    public:
        CommonCopyAndTransposeTester();
        ~CommonCopyAndTransposeTester();

    private:
};

TYPED_TEST_SUITE_P(CommonCopyAndTransposeTester);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
#include "cheetah/data/test_utils/detail/CommonCopyAndTransposeTester.cpp"

#endif // SKA_CHEETAH_CORNER_TURN_TEST_UTILS_COMMONCOPYANDTRANSPOSETESTER_H