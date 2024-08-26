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
#include "cheetah/channel_mask/test/ChannelMaskTest.h"
#include "cheetah/channel_mask/ChannelMask.h"
#include "cheetah/data/TimeFrequency.h"
#include <limits>


namespace ska {
namespace cheetah {
namespace channel_mask {
namespace test {


ChannelMaskTest::ChannelMaskTest()
    : ::testing::Test()
{
}

ChannelMaskTest::~ChannelMaskTest()
{
}

void ChannelMaskTest::SetUp()
{
}

void ChannelMaskTest::TearDown()
{
}

template<typename NumericalRep>
struct TestPolicy {
    public:
        TestPolicy(data::TimeFrequency<Cpu, NumericalRep> const& data)
            : _call_count(0)
            , _expected_data(data)
        {}

        template<typename DataType>
        void operator()(DataType& data) {
            ++_call_count;
            for(auto& sample : data)
            {
                sample = std::numeric_limits<NumericalRep>::max();
            }
            // check data falls in the expected range
            ASSERT_GE(&*data.begin(), &*_expected_data.begin());
            ASSERT_LE(&*data.end(), &*_expected_data.end());
        }

        std::size_t call_count() const { return _call_count; }

    private:
        std::size_t _call_count;
        data::TimeFrequency<Cpu, NumericalRep> const& _expected_data;
};

template<typename NumericalRep>
class TestChannelMask : public ChannelMask<NumericalRep, TestPolicy<NumericalRep>>
{
        typedef ChannelMask<NumericalRep, TestPolicy<NumericalRep>> BaseT;
    public:
        TestChannelMask(Config const& config, data::TimeFrequency<Cpu, NumericalRep> const& data)
            : BaseT(config, data)
        {
        }

        TestPolicy<NumericalRep>& policy() { return this->_policy; }
};

TEST_F(ChannelMaskTest, test_mask)
{
    typedef uint8_t NumericalRep;
    typedef TestChannelMask<NumericalRep> ChannelMaskType;
    typedef data::TimeFrequency<Cpu, NumericalRep> DataType;

    DataType data(data::DimensionSize<data::Frequency>(10)
            , data::DimensionSize<data::Time>(6));
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());

    {
        // no changes expected
        Config config;
        ChannelMaskType mask(config, data);

        mask(data);
        ASSERT_EQ(0U, mask.policy().call_count());
    }
    typedef data::DimensionSpan<data::Frequency> SpanType;
    Config config;
    // take out a single channel
    config.flagged_channels().insert(SpanType(data::DimensionIndex<data::Frequency>(1), data::DimensionSize<data::Frequency>(1)));
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());
    auto channel0=data.channel(0);
    auto channel1=data.channel(1);
    {
        // single contiguos slice
        ChannelMaskType mask(config, data);

        mask(data);
        ASSERT_EQ(1U, mask.policy().call_count());
        // check first channel is clear
        for(auto const& sample : channel0)
        {
            ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
        }
        // channel 1 should be changed
        for(auto const& sample : channel1)
        {
            ASSERT_EQ(std::numeric_limits<NumericalRep>::max(), sample);
        }
        // remaining channels hould not be changed
        for(unsigned channel_num=2; channel_num<10; ++channel_num)
        {
            auto channel=data.channel(channel_num);
            for(auto const& sample : channel)
            {
                ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
            }
        }
    }
    // take out another disjoint channel
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());
    config.flagged_channels().insert(SpanType(data::DimensionIndex<data::Frequency>(4), data::DimensionSize<data::Frequency>(2)));
    {
        // single contiguos slice
        ChannelMaskType mask(config, data);

        mask(data);
        ASSERT_EQ(2U, mask.policy().call_count());
        for(auto const& sample : channel0)
        {
            ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
        }
        // channel 1 shpould be changed
        for(auto const& sample : channel1)
        {
            ASSERT_EQ(std::numeric_limits<NumericalRep>::max(), sample);
        }
        // inteemediate channels 2,3 shpould be unchanged
        for(unsigned channel_num=2; channel_num<4; ++channel_num)
        {
            auto channel=data.channel(channel_num);
            for(auto const& sample : channel)
            {
                ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
            }
        }
        // channels 4,5 should be changed
        for(unsigned channel_num=4; channel_num<6; ++channel_num)
        {
            auto channel=data.channel(channel_num);
            for(auto const& sample : channel)
            {
                ASSERT_EQ(std::numeric_limits<NumericalRep>::max(), sample) << "channel=" << channel_num;
            }
        }
        // remaining channels hould not be changed
        for(unsigned channel_num=6; channel_num<10; ++channel_num)
        {
            auto channel=data.channel(channel_num);
            for(auto const& sample : channel)
            {
                ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
            }
        }
    }
}

TEST_F(ChannelMaskTest, test_ranges_outside_data_size_limits)
{
    typedef uint8_t NumericalRep;
    typedef TestChannelMask<NumericalRep> ChannelMaskType;
    typedef data::TimeFrequency<Cpu, NumericalRep> DataType;
    typedef data::DimensionSpan<data::Frequency> SpanType;

    DataType data(data::DimensionSize<data::Frequency>(5)
            , data::DimensionSize<data::Time>(6));
    // If the span is beyond the dimension then only change channels until the end of the block
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());

    Config config;
    config.flagged_channels().insert(SpanType(data::DimensionIndex<data::Frequency>(8), data::DimensionSize<data::Frequency>(4)));
    {
        // single contiguos slice
        ChannelMaskType mask(config, data);

        mask(data);
        ASSERT_EQ(0U, mask.policy().call_count());
    }
}


TEST_F(ChannelMaskTest, test_ranges_cross_data_size_limits)
{
    typedef uint8_t NumericalRep;
    typedef TestChannelMask<NumericalRep> ChannelMaskType;
    typedef data::TimeFrequency<Cpu, NumericalRep> DataType;
    typedef data::DimensionSpan<data::Frequency> SpanType;

    DataType data(data::DimensionSize<data::Frequency>(10)
            , data::DimensionSize<data::Time>(6));
    // If the span is beyond the dimension then only change channels until the end of the block
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());

    Config config;
    config.flagged_channels().insert(SpanType(data::DimensionIndex<data::Frequency>(8), data::DimensionSize<data::Frequency>(4)));
    {
        // single contiguos slice
        ChannelMaskType mask(config, data);

        mask(data);
        ASSERT_EQ(1U, mask.policy().call_count());

        for(unsigned channel_num=0; channel_num<8; ++channel_num)
        {
            auto channel=data.channel(channel_num);
            for(auto const& sample : channel) {
                ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
            }
        }
        // channels 8 to 10 should be flagged
        for(unsigned channel_num=8; channel_num<10; ++channel_num)
        {
            auto channel=data.channel(channel_num);
            for(auto const& sample : channel) {
                ASSERT_EQ(std::numeric_limits<NumericalRep>::max(), sample);
            }
        }
    }
}

} // namespace test
} // namespace channel_mask
} // namespace cheetah
} // namespace ska
