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
#include "cheetah/data/test/DataSequence2DTest.h"
#include "cheetah/data/DataSequence2D.h"
#include "cheetah/data/DataSequence2DFastIterator.h"
#include "cheetah/data/DataSequence2DPartialIterator.h"
#include <stdio.h>


namespace ska {
namespace cheetah {
namespace data {
namespace test {


DataSequence2DTest::DataSequence2DTest()
{
}

DataSequence2DTest::~DataSequence2DTest()
{
}

void DataSequence2DTest::SetUp(){}
void DataSequence2DTest::TearDown(){}

TEST_F(DataSequence2DTest, test_empty)
{
    data::DataSequence2D<Cpu,uint8_t> empty;
    ASSERT_EQ(empty.fast_axis_length(),0U);
    ASSERT_EQ(empty.slow_axis_length(),0U);
}

TEST_F(DataSequence2DTest, test_create)
{
    const std::size_t x=16;
    const std::size_t y=4;
    data::DataSequence2D<Cpu,uint8_t> ds2d(x,y);
    ASSERT_EQ(x,ds2d.fast_axis_length());
    ASSERT_EQ(y,ds2d.slow_axis_length());
}

TEST_F(DataSequence2DTest, test_fill)
{
    const std::size_t x=16;
    const std::size_t y=4;
    data::DataSequence2D<Cpu,uint8_t> ds2d(x,y,144);
    ASSERT_EQ(x,ds2d.fast_axis_length());
    ASSERT_EQ(y,ds2d.slow_axis_length());
    for(auto v : ds2d){
        ASSERT_EQ(uint8_t(144),v);
    }
}


TEST_F(DataSequence2DTest, test_copy)
{
    const std::size_t x=16;
    const std::size_t y=4;
    data::DataSequence2D<Cpu,uint8_t> ds2d(x,y,138);
    ASSERT_EQ(x,ds2d.fast_axis_length());
    ASSERT_EQ(y,ds2d.slow_axis_length());

    data::DataSequence2D<Cpu,uint8_t> ds2d_copy(x,y);
    std::copy(ds2d.begin(), ds2d.end(), ds2d_copy.begin());
    ASSERT_EQ(x,ds2d_copy.fast_axis_length());
    ASSERT_EQ(y,ds2d_copy.slow_axis_length());
    auto it = ds2d.begin();
    auto it2 = ds2d_copy.begin();
    while (it != ds2d.end()){
        ASSERT_EQ(*it,*it2);
        ++it;
        ++it2;
    }
}


TEST_F(DataSequence2DTest, test_partialfill){
    const std::size_t l=4;
    data::DataSequence2D<Cpu,uint8_t> ds2d(l,l,0);

    auto slice = ds2d.slice(1,3,0,2);

    while (slice != slice.end()){
        *slice = 1;
        ++slice;
    }

    int x=0;
    int y=0;
    for (auto v : ds2d){
        if(x >=1 && x < 3 && y >= 0 && y < 2)
        ASSERT_EQ(1u,v);
        else 
        ASSERT_EQ(0u,v);
        ++x;
        if (x == l){
            x=0;
            ++y;
        }

    }
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
