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
#include "cheetah/data/test/VectorLikeTest.h"
#include "cheetah/data/VectorLike.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {


VectorLikeTest::VectorLikeTest()
    : ::testing::Test()
{
}

VectorLikeTest::~VectorLikeTest()
{
}

void VectorLikeTest::SetUp()
{
}

void VectorLikeTest::TearDown()
{
}

TEST_F(VectorLikeTest, test_erase)
{
    VectorLike<std::vector<std::uint32_t>> test_vector(1024, 1);
    ASSERT_NO_THROW(test_vector.erase(test_vector.begin() + 10));
    static_assert(std::is_same<std::vector<std::uint32_t>::iterator, decltype(test_vector.begin() + 10)>::value,"Types are different");
    auto it  = test_vector.erase(test_vector.begin() + 10);
    ASSERT_EQ(std::distance(test_vector.begin(), it), 10);
    ASSERT_EQ(test_vector.size(),1022);
    ASSERT_NO_THROW(test_vector.erase(test_vector.begin() + 10,test_vector.begin() + 20));
    ASSERT_EQ(test_vector.size(),1012);
}

/*TEST_F(VectorLikeTest, test_const_erase)
{
    VectorLike<std::vector<std::uint32_t>> test_vector(1024, 1);
    ASSERT_NO_THROW(test_vector.erase(test_vector.end()));
    static_assert(std::is_same<std::vector<std::uint32_t>::const_iterator, decltype(test_vector.cbegin() + 10)>::value,"Types are different");
    auto c_it  = test_vector.erase(test_vector.cbegin() + 10);
    ASSERT_EQ(std::distance(test_vector.cbegin(), c_it), 10);
    ASSERT_EQ(test_vector.size(),1022);
    ASSERT_NO_THROW(test_vector.erase(test_vector.cbegin() + 10,test_vector.cbegin() + 20));
    ASSERT_EQ(test_vector.size(),1012);
}*/

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
