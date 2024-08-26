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
#include "cheetah/data/test/RfimFlaggedDataTest.h"
#include "cheetah/data/RfimFlaggedData.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/test_utils/CommonTimeFrequencyTester.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {


RfimFlaggedDataTest::RfimFlaggedDataTest()
    : ::testing::Test()
{
}

RfimFlaggedDataTest::~RfimFlaggedDataTest()
{
}

void RfimFlaggedDataTest::SetUp()
{
}

void RfimFlaggedDataTest::TearDown()
{
}


TEST_F(RfimFlaggedDataTest, test_rfi_flags)
{
    //typedef data::TimeFrequency<Cpu, uint8_t> TF;
    //typedef RfimFlaggedData<TF> TestType;
}

typedef ::testing::Types<CommonTimeFrequencyTesterTraits<data::RfimFlaggedData<data::TimeFrequency<Cpu, uint8_t>>>> RfimFlaggedDataTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(RfimFlags, CommonTimeFrequencyTester, RfimFlaggedDataTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
