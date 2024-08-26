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
#include "cheetah/data/test_utils/DmTimeTest.h"
#include "cheetah/data/DmTime.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

typedef ::testing::Types<
    DmTimeTestTraits<cheetah::Cpu,float>,
    DmTimeTestTraits<cheetah::Cpu,double>,
    DmTimeTestTraits<cheetah::Cpu,char>
    > DmTimeTestTypes;
TYPED_TEST_SUITE(DmTimeTest, DmTimeTestTypes);

TYPED_TEST(DmTimeTest, slice_tests)
{
    typedef Tester<typename TypeParam::Architecture, typename TypeParam::ValueType> Test;
    Test::slice_test(100,13,10);
    Test::slice_test(100,10,10);
    Test::slice_test(10,20,10);
}

TYPED_TEST(DmTimeTest, add_tests)
{
    typedef Tester<typename TypeParam::Architecture, typename TypeParam::ValueType> Test;
    Test::add_test();
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
