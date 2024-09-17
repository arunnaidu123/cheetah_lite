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
#include "cheetah/modules/ddtr/test/AggregationBufferFillerTest.h"
#include "cheetah/modules/ddtr/detail/AggregationBufferFiller.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


AggregationBufferFillerTest::AggregationBufferFillerTest()
    : ::testing::Test()
{
}

AggregationBufferFillerTest::~AggregationBufferFillerTest()
{
}

void AggregationBufferFillerTest::SetUp()
{
}

void AggregationBufferFillerTest::TearDown()
{
}


TEST_F(AggregationBufferFillerTest, test_buffer_handler_call)
{
    unsigned int number_of_channels = 16;
    AggregationBufferFiller<float> buffer([](std::shared_ptr<AggregationBuffer<float>>){ std::cout<<"running handler \n";}, 1024, number_of_channels);
    buffer.set_overlap(256);
    auto object = std::make_shared<FrequencyTimeType>(data::DimensionSize<data::Frequency>(number_of_channels), data::DimensionSize<data::Time>(16));
    for(unsigned channel_num=0; channel_num<16; ++channel_num)
    {
        auto channel=object->channel(channel_num);
        std::fill(channel.begin(), channel.end(), (float)(channel_num));
    }
    for(unsigned int i=0; i<64; ++i) buffer<<object;

    for(unsigned int i=0; i<48; ++i) buffer<<object;
    for(unsigned int i=0; i<49; ++i) buffer<<object;
}


} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska