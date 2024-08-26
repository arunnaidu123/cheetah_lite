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
#include "cheetah/generators/pulse_profile/test/PulsarProfileTest.h"
#include "cheetah/generators/pulse_profile/PulsarProfile.h"
#include <boost/units/systems/si/frequency.hpp>
#include <boost/units/systems/si/prefixes.hpp>


namespace ska {
namespace cheetah {
namespace generators {
namespace test {


PulsarProfileTest::PulsarProfileTest()
    : ::testing::Test()
{
}

PulsarProfileTest::~PulsarProfileTest()
{
}

void PulsarProfileTest::SetUp()
{
}

void PulsarProfileTest::TearDown()
{
}

TEST_F(PulsarProfileTest, test_initialisation)
{
    generators::PulsarProfile p;
    ASSERT_TRUE(p.begin() == p.end());
    double val1=99.0;
    double val2=1099.0;
    p << val1;
    auto it=p.begin();
    ASSERT_FALSE(it == p.end());
    ASSERT_EQ(*it, val1);
    ASSERT_TRUE(++it == p.end());
    p << val2;
    it=p.begin();
    ++it;
    ASSERT_EQ(*it, val2);
}

} // namespace test
} // namespace generators
} // namespace cheetah
} // namespace ska
