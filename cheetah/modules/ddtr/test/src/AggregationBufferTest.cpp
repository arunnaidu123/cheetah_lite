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
#include "cheetah/modules/ddtr/test/AggregationBufferTest.h"
#include "cheetah/modules/ddtr/detail/AggregationBuffer.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


AggregationBufferTest::AggregationBufferTest()
    : ::testing::Test()
{
}

AggregationBufferTest::~AggregationBufferTest()
{
}

void AggregationBufferTest::SetUp()
{
}

void AggregationBufferTest::TearDown()
{
}


TEST_F(AggregationBufferTest, test_buffer_handler_call)
{
    unsigned int number_of_channels = 16;
    AggregationBuffer<float> buffer(1024, number_of_channels);
    AggregationBuffer<float> buffer_temp(1024, number_of_channels);
    ASSERT_EQ(buffer.capacity(), 1024);
    auto object = std::make_shared<FrequencyTimeType>(data::DimensionSize<data::Frequency>(number_of_channels), data::DimensionSize<data::Time>(16));
    for(unsigned channel_num=0; channel_num<16; ++channel_num)
    {
        auto channel=object->channel(channel_num);
        std::fill(channel.begin(), channel.end(), (float)(channel_num));
    }
    for(unsigned int i=0; i<64; ++i) buffer.insert(object);

    buffer.swap(buffer_temp);
}


} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska