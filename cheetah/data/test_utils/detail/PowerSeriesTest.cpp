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
#include "cheetah/data/test_utils/PowerSeriesTest.h"
#include "cheetah/data/PowerSeries.h"
#include "cheetah/data/Units.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template <typename PowerSeriesTestTraitsType>
PowerSeriesTest<PowerSeriesTestTraitsType>::PowerSeriesTest()
    : ::testing::Test()
{
}

template <typename PowerSeriesTestTraitsType>
PowerSeriesTest<PowerSeriesTestTraitsType>::~PowerSeriesTest()
{
}

template <typename PowerSeriesTestTraitsType>
void PowerSeriesTest<PowerSeriesTestTraitsType>::SetUp()
{
}

template <typename PowerSeriesTestTraitsType>
void PowerSeriesTest<PowerSeriesTestTraitsType>::TearDown()
{
}

template <typename TypeParam>
void SampleCountTest<TypeParam>::test(std::size_t nbins)
{
    std::size_t number_of_bins = nbins;
    FourierFrequencyType df = 5.4 * hz;
    double dof = 5.0;
    PowerSeries<typename TypeParam::Architecture,typename TypeParam::ValueType> fseries(df,dof);
    fseries.resize(number_of_bins);
    ASSERT_EQ(fseries.size(),number_of_bins);
    ASSERT_EQ(fseries.frequency_step(),df);
    ASSERT_EQ(fseries.degrees_of_freedom(),dof);
}



} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
