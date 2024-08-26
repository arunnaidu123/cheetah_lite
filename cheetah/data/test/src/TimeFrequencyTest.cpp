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
#include "cheetah/data/test/TimeFrequencyTest.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/test_utils/CommonTimeFrequencyTester.h"
#include "cheetah/data/test_utils/CommonCopyAndTransposeTester.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

TimeFrequencyTest::TimeFrequencyTest()
    : ::testing::Test()
{
}

TimeFrequencyTest::~TimeFrequencyTest()
{
}

void TimeFrequencyTest::SetUp()
{
}

void TimeFrequencyTest::TearDown()
{
}


typedef ::testing::Types
    < CommonTimeFrequencyTesterTraits<data::TimeFrequency<Cpu, uint8_t>>
    , CommonTimeFrequencyTesterTraits<data::TimeFrequency<Cpu, uint16_t>>
    , CommonTimeFrequencyTesterTraits<data::TimeFrequency<Cpu, float>>
    >  TFTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TimeFrequency, CommonTimeFrequencyTester, TFTypes);

typedef ::testing::Types
    < CommonCopyAndTransposeTesterTraits<data::TimeFrequency<Cpu, uint8_t>, data::FrequencyTime<Cpu, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::TimeFrequency<Cpu, uint16_t>, data::FrequencyTime<Cpu, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::TimeFrequency<Cpu, uint16_t>, data::FrequencyTime<Cpu, uint16_t>>
    > TFCornerTurnTesterTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(TimeFrequency, CommonCopyAndTransposeTester, TFCornerTurnTesterTypes);


// check dimension query structs work
static_assert(pss::astrotypes::has_dimensions<TimeFrequency<Cpu, uint8_t>, data::Time, data::Frequency>::value, "expecting true");
static_assert(pss::astrotypes::has_exact_dimensions<TimeFrequency<Cpu, uint8_t>, data::Time, data::Frequency>::value, "expecting true");
static_assert(!pss::astrotypes::has_exact_dimensions<TimeFrequency<Cpu, uint8_t>, data::Frequency, data::Time>::value, "expecting false");

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
