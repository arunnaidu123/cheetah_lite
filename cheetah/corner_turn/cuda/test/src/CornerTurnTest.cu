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

#include "cheetah/corner_turn/cuda/test/CornerTurnTest.h"
#include "cheetah/corner_turn/test_utils/CornerTurnTester.h"
#include "cheetah/corner_turn/cuda/CornerTurn.h"

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace cuda {
namespace test {

template<typename SrcT, typename DstT>
struct CornerTurnTesterTraits
{
    typedef SrcT SrcType;
    typedef DstT DstType;
    typedef Cuda Architecture;

    template<typename... Args>
    static inline
    void corner_turn(Args&&... args)
    {
        cuda::corner_turn(std::forward<Args>(args)...);
    }
};

} // namespace test
} // cuda
} // namespace corner_turn
} // namespace cheetah
} // namespace ska

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace test {

typedef ::testing::Types
    < corner_turn::cuda::test::CornerTurnTesterTraits<uint8_t, uint8_t>
    , corner_turn::cuda::test::CornerTurnTesterTraits<uint16_t, uint8_t>
    , corner_turn::cuda::test::CornerTurnTesterTraits<float, uint8_t>
    , corner_turn::cuda::test::CornerTurnTesterTraits<uint8_t, uint16_t>
    , corner_turn::cuda::test::CornerTurnTesterTraits<uint16_t, uint16_t>
    , corner_turn::cuda::test::CornerTurnTesterTraits<float, uint16_t>
    , corner_turn::cuda::test::CornerTurnTesterTraits<uint8_t, float>
    , corner_turn::cuda::test::CornerTurnTesterTraits<uint16_t, float>
    , corner_turn::cuda::test::CornerTurnTesterTraits<float, float>
    >  CudaTesterTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(CornerTurnTest, CornerTurnTester, CudaTesterTypes);

} // namespace test
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
