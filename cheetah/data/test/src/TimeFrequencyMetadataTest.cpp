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
#include "cheetah/data/test/TimeFrequencyMetadataTest.h"
#include "cheetah/data/TimeFrequencyMetadata.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {


TimeFrequencyMetadataTest::TimeFrequencyMetadataTest()
    : ::testing::Test()
{
}

TimeFrequencyMetadataTest::~TimeFrequencyMetadataTest()
{
}

void TimeFrequencyMetadataTest::SetUp()
{
}

void TimeFrequencyMetadataTest::TearDown()
{
}

TEST_F(TimeFrequencyMetadataTest, test_start_time)
{
    typedef TimeFrequencyMetadata DataType;

    DataType meta;
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    typename DataType::TimeIntervalType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    meta.sample_interval(interval);
    meta.start_time(epoch);
    ASSERT_EQ(meta.start_time(), epoch);
}

TEST_F(TimeFrequencyMetadataTest, test_start_time_offset)
{
    typedef TimeFrequencyMetadata DataType;

    DataType meta;
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    typename DataType::TimeIntervalType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    meta.sample_interval(interval);
    meta.start_time(epoch);
    epoch += std::chrono::duration<double>(meta.sample_interval().value()*std::size_t(12));
    ASSERT_EQ(meta.start_time(12L), epoch);
}

TEST_F(TimeFrequencyMetadataTest, test_copy)
{
    typedef TimeFrequencyMetadata DataType;
    data::DimensionSize<data::Frequency> number_of_channels(10U);

    DataType meta;
    auto f1 =  typename DataType::FrequencyType(10.0 * boost::units::si::hertz);
    auto f2 =  typename DataType::FrequencyType(20.0 * boost::units::si::hertz);
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);

    meta.channel_frequencies_const_width( f1, delta, number_of_channels );
    meta.start_time(epoch);

    DataType meta_copy(meta);
    ASSERT_EQ(meta_copy.channel_frequencies().size(), number_of_channels);
    for(std::size_t i =0; i < number_of_channels; ++i) {
        ASSERT_EQ(meta_copy.channel_frequencies()[i], meta.channel_frequencies()[i]);
    }
    ASSERT_EQ(meta_copy.start_time(), meta.start_time());
}

TEST_F(TimeFrequencyMetadataTest, test_low_high_freq)
{
    typedef TimeFrequencyMetadata DataType;
    data::DimensionSize<data::Frequency> number_of_channels(10U);

    DataType meta;
    typedef std::pair<typename DataType::FrequencyType, typename DataType::FrequencyType> ReturnType;

    // eexpect 0 values if channel_freq are not set
    ReturnType r = meta.low_high_frequencies();
    ASSERT_EQ(0.0, r.first.value());
    ASSERT_EQ(0.0, r.second.value());

    auto f1 =  typename DataType::FrequencyType(10.0 * boost::units::si::hertz);
    auto f2 =  typename DataType::FrequencyType(20.0 * boost::units::si::hertz);
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);

    // low to haigh channel freq ordering
    meta.channel_frequencies_const_width( f1, delta, number_of_channels );
    ASSERT_EQ(meta.channel_frequencies().size(), number_of_channels);
    r = meta.low_high_frequencies();
    ASSERT_EQ(f1, r.first);
    ASSERT_EQ(f2, r.second);

    // high to low metareq ordering
    meta.channel_frequencies_const_width( f2, -1.0 *delta, number_of_channels );
    ASSERT_EQ(meta.channel_frequencies().size(), number_of_channels);
    r = meta.low_high_frequencies();
    ASSERT_EQ(f1, r.first);
    ASSERT_EQ(f2, r.second);

}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
