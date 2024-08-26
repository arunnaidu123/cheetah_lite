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
#include "cheetah/modules/sps/cpu/test/ConfigTest.h"
#include "cheetah/modules/sps/cpu/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace sps {
namespace cpu {
namespace test {


ConfigTest::ConfigTest()
    : ::testing::Test()
{
}

ConfigTest::~ConfigTest()
{
}

void ConfigTest::SetUp()
{
}

void ConfigTest::TearDown()
{
}

TEST_F(ConfigTest, test_activate)
{
    Config config;
    ASSERT_FALSE(config.active());
    config.activate();
    ASSERT_TRUE(config.active());
}

} // namespace test
} // namespace cpu
} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
