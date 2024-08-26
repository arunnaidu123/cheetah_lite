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
#include "cheetah/data/test/DedispersionMeasureTest.h"
#include "cheetah/data/DedispersionMeasure.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {


DedispersionMeasureTest::DedispersionMeasureTest()
    : ::testing::Test()
{
}

DedispersionMeasureTest::~DedispersionMeasureTest()
{
}

void DedispersionMeasureTest::SetUp()
{
}

void DedispersionMeasureTest::TearDown()
{
}

TEST_F(DedispersionMeasureTest, test_instantiation)
{
    data::DedispersionMeasureType<float> dm_float(1.6 * data::parsecs_per_cube_cm);
    data::DedispersionMeasureType<double> dm_dbl(1.0 * data::parsec_per_cube_cm);
    dm_float + dm_dbl;
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
