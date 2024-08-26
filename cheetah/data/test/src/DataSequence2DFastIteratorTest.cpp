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
#include "cheetah/data/test/DataSequence2DFastIteratorTest.h"
#include "cheetah/data/DataSequence2DFastIterator.h"
#include "cheetah/data/DataSequence2D.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {


DataSequence2DFastIteratorTest::DataSequence2DFastIteratorTest()
    : ::testing::Test()
{
}

DataSequence2DFastIteratorTest::~DataSequence2DFastIteratorTest()
{
}

void DataSequence2DFastIteratorTest::SetUp()
{
}

void DataSequence2DFastIteratorTest::TearDown()
{
}

TEST_F(DataSequence2DFastIteratorTest, test_operator_equal)
{
    int val = 99;
    cheetah::data::DataSequence2D<Cpu, int> data(2, 1, val);
    cheetah::data::DataSequence2DFastIterator<Cpu, int> it_1(data);
    cheetah::data::DataSequence2DFastIterator<Cpu, int> it_2(data);
    ASSERT_EQ(it_1, it_2++);
    ASSERT_NE(it_1++, it_2);
}

TEST_F(DataSequence2DFastIteratorTest, test_empty_set)
{
    cheetah::data::DataSequence2D<Cpu, int> data;
    cheetah::data::DataSequence2DFastIterator<Cpu, int> it(data);
    ASSERT_EQ(it, data.end());
}

TEST_F(DataSequence2DFastIteratorTest, test_deref)
{
    int val = 99;
    cheetah::data::DataSequence2D<Cpu, int> data(1,1, val);
    cheetah::data::DataSequence2DFastIterator<Cpu, int> it(data);
    ASSERT_NE(it, data.end());
    ASSERT_EQ(val, *it);
    ++it;
    ASSERT_EQ(it, data.end());
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
