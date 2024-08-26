/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022-2024 The SKA organisation
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
#ifndef SKA_CHEETAH_DATA_TEST_OPTIMISEDCANDIDATETEST_H
#define SKA_CHEETAH_DATA_TEST_OPTIMISEDCANDIDATETEST_H

#include "cheetah/data/OptimisedCandidate.h"
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

/**
 * @brief Class to test the OptimisedCandidate class
 * @details
 */

template<typename TypeParam>
class OptimisedCandidateTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

        // A candidate data cube to be used in most unit tests.
        // Values for TypeParam are listed in OptimisedCandidateTestTypes below.
        TypeParam *_candidate;

    public:
        OptimisedCandidateTest() = default;
};

using OptimisedCandidateTestTypes = ::testing::Types<
                                        data::OptimisedCandidate<uint8_t>
                                        , data::OptimisedCandidate<uint16_t>
                                        , const data::OptimisedCandidate<uint8_t>
                                        , const data::OptimisedCandidate<uint16_t>
                                    >;
TYPED_TEST_SUITE(OptimisedCandidateTest, OptimisedCandidateTestTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_TEST_OPTIMISEDCANDIDATETEST_H
