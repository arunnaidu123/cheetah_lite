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

#ifndef SKA_CHEETAH_CORNER_TURN_TEST_UTILS_CORNERTURNTESTER_H
#define SKA_CHEETAH_CORNER_TURN_TEST_UTILS_CORNERTURNTESTER_H

#include "cheetah/corner_turn/CornerTurn.h"
#include <utility>
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace test {

/**
 * @brief Generic corner turn tester for different Architectures
 * @details Takes in input and output test container and compares both input and output.
 * @tparam CornerTurnTesterTraitsT
 *         Requires the following typedefs:
 *            SrcType : the input containers value_type
 *            DstType : the output cantainers value_type
 *            Architecture : the Architecture on which to test
 *         Requires the following function to be defined which
 *         should call the function you are trying to test
 *         with the provided arguments.
 *@code{.cpp}
 *           template<typename... Args>
 *           static inline // optionally static
 *           void corner_turn(Args&&... args);
 *@endcode
 * ## Instantiating the test suite
 * The Tester is based on the google test typed parameter framework.
 * Thus to instantiate the test you will need to provide the
 * INSTANTIATE_TYPED_TEST_SUITE_P macro with a unique name for your tests
 * along with a ::testing::Types<> where the template parameters
 * provided are a list of CornerTurnTesterTraitsT that define the
 * use cases you want to exercise.
 * Note that this INSTANTIATE_TYPED_TEST_SUITE_P has to be in the same namespace
 * as this tester.
 *
 * ~~~~{.cpp}
 * typedef ::testing::Types
 *  < corner_turn::cpu::test::CornerTurnTesterTraits<uint8_t, uint8_t>
 *  , corner_turn::cpu::test::CornerTurnTesterTraits<uint16_t, uint8_t>
 *  , corner_turn::cpu::test::CornerTurnTesterTraits<float, float>
 *  > MyAlgoTesterTypes;
 * INSTANTIATE_TYPED_TEST_SUITE_P(MyAlgoTests, CornerTurnTester, MyAlgoTesterTypes);
 * ~~~~
 */

template <typename CornerTurnTesterTraitsT>
class CornerTurnTester : public ::testing::TestWithParam<CornerTurnTesterTraitsT>
{
        typedef ::testing::TestWithParam<CornerTurnTesterTraitsT> BaseT;

    public:
        CornerTurnTester();
        ~CornerTurnTester() = default;
};

TYPED_TEST_SUITE_P(CornerTurnTester);

} // namespace test
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
#include "cheetah/corner_turn/test_utils/detail/CornerTurnTester.cpp"

#endif // SKA_CHEETAH_CORNER_TURN_TEST_UTILS_CORNERTURNTESTER_H
