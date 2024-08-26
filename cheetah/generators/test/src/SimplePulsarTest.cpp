/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2024 The SKA organisation
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
#include "cheetah/generators/test/SimplePulsarTest.h"
#include "cheetah/generators/SimplePulsar.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"



namespace ska {
namespace cheetah {
namespace generators {
namespace test {


SimplePulsarTest::SimplePulsarTest()
    : ::testing::Test()
{
}

SimplePulsarTest::~SimplePulsarTest()
{
}

void SimplePulsarTest::SetUp()
{
}

void SimplePulsarTest::TearDown()
{
}

TEST_F(SimplePulsarTest, test_default_config)
{
    generators::SimplePulsarConfig config;
    generators::SimplePulsar<data::TimeFrequency<Cpu, uint8_t>> pulsar_generator(config);
    data::TimeFrequency<Cpu, uint8_t> data(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(100));
    pulsar_generator.next(data);
}

} // namespace test
} // namespace generators
} // namespace cheetah
} // namespace ska
