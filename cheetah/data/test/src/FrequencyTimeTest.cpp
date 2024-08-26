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
#include "cheetah/data/test/FrequencyTimeTest.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/data/test_utils/CommonTimeFrequencyTester.h"
#include "cheetah/data/test_utils/CommonCopyAndTransposeTester.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

FrequencyTimeTest::FrequencyTimeTest()
    : ::testing::Test()
{
}

FrequencyTimeTest::~FrequencyTimeTest()
{
}

void FrequencyTimeTest::SetUp()
{
}

void FrequencyTimeTest::TearDown()
{
}

typedef ::testing::Types
    < CommonTimeFrequencyTesterTraits<data::FrequencyTime<Cpu, uint8_t>>
    , CommonTimeFrequencyTesterTraits<data::FrequencyTime<Cpu, uint16_t>>
    , CommonTimeFrequencyTesterTraits<data::FrequencyTime<Cpu, float>>
    >  FtTesterTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(FrequencyTime, CommonTimeFrequencyTester, FtTesterTypes);

typedef ::testing::Types
    < CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::FrequencyTime<Cpu, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::TimeFrequency<Cpu, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::TimeFrequency<Cpu, uint16_t>>
    > FTCornerTurnTesterTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(FrequencyTime, CommonCopyAndTransposeTester, FTCornerTurnTesterTypes);

// check dimension query structs work
static_assert(pss::astrotypes::has_dimensions<FrequencyTime<Cpu, uint8_t>, data::Frequency, data::Time>::value, "expecting true");
static_assert(pss::astrotypes::has_exact_dimensions<FrequencyTime<Cpu, float>, data::Frequency, data::Time>::value, "expecting true");
static_assert(!pss::astrotypes::has_exact_dimensions<FrequencyTime<Cpu, uint8_t>, data::Time, data::Frequency>::value, "expecting false");

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
