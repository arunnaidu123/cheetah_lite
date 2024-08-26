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
#include "cheetah/data/test/TimeFrequencyFlagsTest.h"
#include "cheetah/data/TimeFrequencyFlags.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {


TimeFrequencyFlagsTest::TimeFrequencyFlagsTest()
    : ::testing::Test()
{
}

TimeFrequencyFlagsTest::~TimeFrequencyFlagsTest()
{
}

void TimeFrequencyFlagsTest::SetUp()
{
}

void TimeFrequencyFlagsTest::TearDown()
{
}

TEST_F(TimeFrequencyFlagsTest, test_meta)
{
    DimensionSize<Time> number_of_spectra(10U);
    DimensionSize<Frequency> number_of_channels(5U);
    data::TimeFrequencyFlags<Cpu> tf(number_of_spectra, number_of_channels);
    ASSERT_EQ(number_of_spectra, tf.number_of_spectra());
    ASSERT_EQ(number_of_channels, tf.number_of_channels());
}

TEST_F(TimeFrequencyFlagsTest, test_time_sample)
{
    DimensionSize<Time> number_of_spectra(10U);
    DimensionSize<Frequency> number_of_channels(5U);
    data::TimeFrequencyFlags<Cpu> tf(number_of_spectra, number_of_channels);
    for(unsigned n=0; n< number_of_spectra; ++n) {
        auto time_sample = tf.spectrum(n);
        auto const_time_sample = static_cast<const data::TimeFrequencyFlags<Cpu>>(tf).spectrum(n);
        ASSERT_EQ(number_of_channels, time_sample.end() - time_sample.begin());
        ASSERT_EQ(number_of_channels, const_time_sample.end() - const_time_sample.begin());
    }
}

TEST_F(TimeFrequencyFlagsTest, test_begin_end_reset)
{
    DimensionSize<Time> number_of_spectra(10U);
    DimensionSize<Frequency> number_of_channels(5U);
    data::TimeFrequencyFlags<Cpu> tf(number_of_spectra, number_of_channels);
    tf.reset(false);
    auto it = tf.begin();

    ASSERT_EQ(number_of_channels * number_of_spectra, tf.end() - it);
    std::size_t count = 0;
    while(it != tf.end()) {
        ASSERT_FALSE(*it);
        ++count;
        ++it;
    }
    ASSERT_EQ(std::size_t(number_of_channels * number_of_spectra), count);

    // set all to true
    tf.reset(true);
    count = 0;
    it = tf.begin();
    while(it != tf.end()) {
        ASSERT_TRUE(*it);
        ++count;
        ++it;
    }
    ASSERT_EQ(std::size_t(number_of_channels * number_of_spectra), count);
}

TEST_F(TimeFrequencyFlagsTest, test_channel)
{
    DimensionSize<Time> number_of_spectra(10U);
    DimensionSize<Frequency> number_of_channels(5U);
    data::TimeFrequencyFlags<Cpu> tf(number_of_spectra, number_of_channels);
    tf.reset(false);
    for(unsigned n=0; n< number_of_channels; ++n) {
        auto channel_sample = tf.channel(n);
        auto it = channel_sample.begin();
        std::size_t count=0;
        while(it != channel_sample.end()) {
            *it = true;
            ++count;
            ++it;
        }
        ASSERT_EQ(number_of_spectra, count);
        ASSERT_EQ(number_of_spectra, channel_sample.end() - channel_sample.begin());
        //ASSERT_EQ(number_of_spectra, std::distance(channel_sample.begin() , channel_sample.end()));
        //
        // verify we have set the value and reset to zero
        for(unsigned nv=0; nv< number_of_channels; ++nv) {
            auto channel_sample = tf.channel(nv);
            auto it = channel_sample.begin();
            while(it != channel_sample.end()) {
                if(nv==n) {
                    ASSERT_TRUE(*it);
                }
                else {
                    ASSERT_FALSE(*it);
                }
                ++it;
            }
        }
        // reset the channel
        it = channel_sample.begin();
        while(it != channel_sample.end()) {
            *it = false;
            ++it;
        }
    }
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
