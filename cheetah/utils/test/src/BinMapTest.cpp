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
#include "cheetah/utils/test/BinMapTest.h"
#include "cheetah/utils/BinMap.h"


namespace ska {
namespace cheetah {
namespace utils {
namespace test {


BinMapTest::BinMapTest()
    : ::testing::Test()
{
}

BinMapTest::~BinMapTest()
{
}

void BinMapTest::SetUp()
{
}

void BinMapTest::TearDown()
{
}

TEST_F(BinMapTest, test_hash)
{
    // Use Case:
    // two identical maps should hash to the same value
    BinMap<double> map1(32*256);
    BinMap<double> map2(32*256);
    std::size_t id1=std::hash<utils::BinMap<double>>()(map1);
    std::size_t id2=std::hash<utils::BinMap<double>>()(map2);
    ASSERT_EQ(id1,id2);
    map2.set_lower_bound(10030.012);
    ASSERT_NE(id1,  std::hash<utils::BinMap<double>>()(map2));
    map1.set_lower_bound(10030.012);
    id1=std::hash<utils::BinMap<double>>()(map1);
    ASSERT_EQ(id1, std::hash<utils::BinMap<double>>()(map2));
}

TEST_F(BinMapTest, test_double_map)
{
    utils::BinMap<double> map(32*256);
    double start = 142.779541;
    double width = 0.006104;
    map.set_lower_bound(start);
    map.set_bin_width(width);
    ASSERT_DOUBLE_EQ( start + width/2.0, map.bin_assignment_number(0));
    ASSERT_DOUBLE_EQ( start , map.bin_start(0));
    ASSERT_DOUBLE_EQ( start , map.lower_bound());
    ASSERT_DOUBLE_EQ( start + width , map.bin_end(0));
    ASSERT_DOUBLE_EQ( start + width/2.0 + width , map.bin_assignment_number(1));
    ASSERT_DOUBLE_EQ( start + width/2.0 + 2*width , map.bin_assignment_number(2));
    ASSERT_EQ( 0U , map.bin_index(start) );
    ASSERT_EQ( 1U , map.bin_index(start + width) );
}

TEST_F(BinMapTest, test_negative_width)
{
    utils::BinMap<double> map(32*256);
    double start = 142.779541;
    double width = 0.006104;
    map.set_lower_bound(start);
    map.set_bin_width(-1.0*width);
    ASSERT_DOUBLE_EQ( start - width/2.0, map.bin_assignment_number(0));
    ASSERT_DOUBLE_EQ( start , map.bin_start(0));
    ASSERT_DOUBLE_EQ( start , map.lower_bound());
    ASSERT_DOUBLE_EQ( start - width , map.bin_end(0));
    ASSERT_DOUBLE_EQ( start - width/2.0 - width , map.bin_assignment_number(1));
    ASSERT_DOUBLE_EQ( start - width/2.0 - 2*width , map.bin_assignment_number(2));
    ASSERT_EQ( 0U , map.bin_index(start) );
    ASSERT_EQ( 1U , map.bin_index(start - width) );
}

TEST_F(BinMapTest, test_bin_width)
{
    { // Use Case : Single channel - bin_width explicitly set
        utils::BinMap<int> map(1);
        int start = 1779541;
        map.set_lower_bound(start);
        int width=2;
        map.set_bin_width(width);
        ASSERT_EQ(width, map.bin_width());
    }
    { // Use Case : Single channel - bin_width set by setting start and end only
        utils::BinMap<int> map(1);
        int start = 1779541;
        int width=2;
        int end = start + width;
        map.set_lower_bound(start);
        map.set_upper_bound(end);
        ASSERT_EQ(width, map.bin_width());
    }
}

} // namespace test
} // namespace utils
} // namespace cheetah
} // namespace ska
