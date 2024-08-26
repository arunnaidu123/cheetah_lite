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
#include "cheetah/channel_mask/test/ConfigurableChannelMaskTest.h"
#include "cheetah/channel_mask/ConfigurableChannelMask.h"


namespace ska {
namespace cheetah {
namespace channel_mask {
namespace test {


ConfigurableChannelMaskTest::ConfigurableChannelMaskTest()
    : ::testing::Test()
{
}

ConfigurableChannelMaskTest::~ConfigurableChannelMaskTest()
{
}

void ConfigurableChannelMaskTest::SetUp()
{
}

void ConfigurableChannelMaskTest::TearDown()
{
}

TEST_F(ConfigurableChannelMaskTest, test_mask_unset_type)
{
    typedef uint8_t NumericalRep;
    typedef data::TimeFrequency<Cpu, uint8_t> DataType;
    typedef data::DimensionSpan<data::Frequency> SpanType;

    // configuration
    ConfigurableChannelMaskConfig<NumericalRep> config;
    config.policy_name() = "";
    config.flagged_channels().insert(SpanType(data::DimensionIndex<data::Frequency>(1), data::DimensionSize<data::Frequency>(1)));

    // data
    DataType data(data::DimensionSize<data::Frequency>(10)
            , data::DimensionSize<data::Time>(6));
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());

    ConfigurableChannelMask<NumericalRep> mask(config);
    mask(data);
    for(auto const& sample : data) {
        ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
    }
}

TEST_F(ConfigurableChannelMaskTest, test_mask_replace_constant_type)
{
    typedef uint8_t NumericalRep;
    typedef data::TimeFrequency<Cpu, uint8_t> DataType;
    typedef data::DimensionSpan<data::Frequency> SpanType;

    // configuration
    ConfigurableChannelMaskConfig<NumericalRep> config;
    config.policy_name() = "replace_constant";

    config.flagged_channels().insert(SpanType(data::DimensionIndex<data::Frequency>(1), data::DimensionSize<data::Frequency>(1)));

    // data
    DataType data(data::DimensionSize<data::Frequency>(10)
            , data::DimensionSize<data::Time>(6));
    std::fill(data.begin(), data.end(), std::numeric_limits<NumericalRep>::min());

    ConfigurableChannelMask<NumericalRep> mask(config);
    mask(data);
    for(auto const& sample : data.channel(0)) {
        ASSERT_EQ(std::numeric_limits<NumericalRep>::min(), sample);
    }
    for(auto const& sample : data.channel(1)) {
        ASSERT_NE(std::numeric_limits<NumericalRep>::min(), sample);
    }
}

} // namespace test
} // namespace channel_mask
} // namespace cheetah
} // namespace ska
