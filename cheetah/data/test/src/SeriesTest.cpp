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
#include "cheetah/data/test/SeriesTest.h"
#include "cheetah/data/test_utils/SeriesTester.h"
#include <panda/test/TestArch.h>


namespace ska {
namespace cheetah {
namespace data {
namespace test {


SeriesTest::SeriesTest()
    : ::testing::Test()
{
}

SeriesTest::~SeriesTest()
{
}

void SeriesTest::SetUp()
{
}

void SeriesTest::TearDown()
{
}

template<typename ValueType>
class CpuSeriesTestTraits : public SeriesTesterTraits<data::Series<panda::Cpu, ValueType>>
{
        typedef SeriesTesterTraits<data::Series<panda::Cpu, ValueType>> BaseT;
        typedef typename BaseT::Allocator Allocator;

    public:
        Allocator allocator(panda::PoolResource<panda::Cpu>&) override { return Allocator(); }
};

typedef ::testing::Types<CpuSeriesTestTraits<float>, CpuSeriesTestTraits<uint8_t>> CpuSeriesTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(BasicCpuSeries, SeriesTester, CpuSeriesTraitsTypes);

template<typename ValueType>
class TestArchSeriesTestTraits : public SeriesTesterTraits<data::Series<panda::test::TestArch, ValueType>>
{
        typedef SeriesTesterTraits<data::Series<panda::test::TestArch, ValueType>> BaseT;
        typedef typename BaseT::Allocator Allocator;

    public:
        Allocator allocator(panda::PoolResource<panda::test::TestArch>& device) override { return Allocator(device); }
};

typedef ::testing::Types<TestArchSeriesTestTraits<double>
                        , TestArchSeriesTestTraits<float>
                        , TestArchSeriesTestTraits<uint8_t>> TestArchSeriesTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TestArchSeries, SeriesTester, TestArchSeriesTraitsTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
