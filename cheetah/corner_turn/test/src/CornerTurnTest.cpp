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
#include "cheetah/corner_turn/test/CornerTurnTest.h"
#include "cheetah/corner_turn/test_utils/CornerTurnTester.h"
#include "cheetah/corner_turn/CornerTurn.h"
#include <panda/AlgorithmInfo.h>


namespace ska {
namespace cheetah {
namespace corner_turn {
namespace test {

/**
 * @brief testTraits for exercising the top level CornerTurn api based on Arch type
 * @details Generic traits and may need to modified for different architectures.
 * @tparam AlgoT : Architecture used
 * @tparam SrcT : value_type inside the source container
 * @tparam DstT : value_type inside the destination container
 */
template <class ArchT, typename SrcT, typename DstT>
struct CornerTurnTesterTopApiTraits
{
    typedef ArchT Architecture;
    typedef SrcT SrcType;
    typedef DstT DstType;

    template<typename... Args>
    static inline
    void corner_turn(Args&&... args)
    {
        corner_turn::corner_turn(std::forward<Args>(args)...);
    }
};

// helper to generate the required tests from a list of Architecture types
template<typename ArchTuple>
struct TesterTypesExpander;

template<typename... ArchTs>
struct TesterTypesExpander<std::tuple<ArchTs...>> {
    typedef ::testing::Types<CornerTurnTesterTopApiTraits<ArchTs, uint8_t, uint8_t>...
                           , CornerTurnTesterTopApiTraits<ArchTs, uint16_t, uint8_t>...
                           > type;
};

// generate test set from the current ActiveAlgos tuple set in the Algorithms.h file
typedef typename TesterTypesExpander<typename panda::MixedAlgoTraits<corner_turn::ActiveAlgos>::Architectures>::type ApiTesterTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(ApiCornerTurnTest, CornerTurnTester, ApiTesterTypes);

} // namespace test
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
