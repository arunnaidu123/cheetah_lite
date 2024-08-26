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
#include "cheetah/channel_mask/test/ReplaceConstantPolicyTest.h"
#include "cheetah/channel_mask/ReplaceConstantPolicy.h"
#include "cheetah/channel_mask/ReplaceConstantPolicyConfig.h"
#include "cheetah/data/TimeFrequency.h"


namespace ska {
namespace cheetah {
namespace channel_mask {
namespace test {


ReplaceConstantPolicyTest::ReplaceConstantPolicyTest()
    : ::testing::Test()
{
}

ReplaceConstantPolicyTest::~ReplaceConstantPolicyTest()
{
}

void ReplaceConstantPolicyTest::SetUp()
{
}

void ReplaceConstantPolicyTest::TearDown()
{
}

template<typename NumericalRep>
void test_policy() {
    typedef data::TimeFrequency<Cpu, NumericalRep> DataType;
    ReplaceConstantPolicyConfig<uint8_t> config;
    ReplaceConstantPolicy<uint8_t> policy(config);

    {
        // entire chunk
        DataType data( data::DimensionSize<data::Frequency>(10)
                     , data::DimensionSize<data::Time>(6));
        std::fill(data.begin(), data.end(), 0);
        policy(data);
        for(auto const& sample : data) {
            ASSERT_EQ(sample, config.value());
        }
    }

    {
        // single channel
        DataType data(data::DimensionSize<data::Frequency>(10)
                         , data::DimensionSize<data::Time>(6));
        std::fill(data.begin(), data.end(), 0);

        auto channel=data.channel(1);
        for(auto const& sample : channel)
        {
            ASSERT_NE(sample, config.value());
        }
        policy(channel);
        for(auto const& sample : channel)
        {
            ASSERT_EQ(sample, config.value());
        }
    }

    {
        // a slice within the chunk
        DataType data(data::DimensionSize<data::Frequency>(10)
                         , data::DimensionSize<data::Time>(6));
        std::fill(data.begin(), data.end(), 0);

        auto slice=data.slice(data::DimensionSpan<data::Frequency>(data::DimensionIndex<data::Frequency>(2), data::DimensionIndex<data::Frequency>(4)));
        for(auto const& sample : slice)
        {
            ASSERT_NE(sample, config.value());
        }
        policy(slice);
        for(auto const& sample : slice)
        {
            ASSERT_EQ(sample, config.value());
        }
    }
}

TEST_F(ReplaceConstantPolicyTest, test_policy_uint8_t)
{
    test_policy<uint8_t>();
}

} // namespace test
} // namespace channel_mask
} // namespace cheetah
} // namespace ska
