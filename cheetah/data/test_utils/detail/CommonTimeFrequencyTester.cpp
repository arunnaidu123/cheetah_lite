/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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

namespace ska {
namespace cheetah {
namespace data {
namespace test {


template <typename TestTraits>
CommonTimeFrequencyTester<TestTraits>::CommonTimeFrequencyTester()
{
}

template <typename TestTraits>
CommonTimeFrequencyTester<TestTraits>::~CommonTimeFrequencyTester()
{
}

template <typename TestTraits>
void CommonTimeFrequencyTester<TestTraits>::SetUp()
{
}

template <typename TestTraits>
void CommonTimeFrequencyTester<TestTraits>::TearDown()
{
}

/**
* @test CommonTimeFrequencyTester::test_dimensions generic tester for all the TF and FT data types.
* @given Input (TF or FT) data types
* @when when the dimentions are set.
* @then test simply checks if the dimentions are set as expected.
*/
ALGORITHM_TYPED_TEST_P(CommonTimeFrequencyTester, test_dimensions)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    data::DimensionSize<data::Time> number_of_spectra(10);
    data::DimensionSize<data::Frequency> number_of_channels(10);
    DataType tf_1(number_of_channels, number_of_spectra);
    DataType tf_2(number_of_spectra, number_of_channels);

    ASSERT_EQ(tf_1.number_of_channels(), number_of_channels);
    ASSERT_EQ(tf_2.number_of_channels(), number_of_channels);
    ASSERT_EQ(tf_1.number_of_spectra(), number_of_spectra);
    ASSERT_EQ(tf_2.number_of_spectra(), number_of_spectra);
}

/**
* @test CommonTimeFrequencyTester::test_samples generic tester for all the TF and FT data types.
* @given Input (TF or FT) data types
* @when when the dimentions are set.
* @then test simply checks if spectrum has expected number of channels.
*/
ALGORITHM_TYPED_TEST_P(CommonTimeFrequencyTester, test_samples)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    data::DimensionSize<data::Time> number_of_spectra(10U);
    data::DimensionSize<data::Frequency> number_of_channels(5U);

    DataType tf(number_of_spectra, number_of_channels);
    ASSERT_EQ(static_cast<std::size_t>(number_of_spectra), tf.number_of_spectra());
    ASSERT_EQ(static_cast<std::size_t>(number_of_channels), tf.number_of_channels());

    for(unsigned sample_number=0; sample_number < tf.number_of_spectra(); ++sample_number) {
        SCOPED_TRACE("sample = " + std::to_string(sample_number));
        typename DataType::Spectra it = tf.spectrum(sample_number);
        unsigned channel_count = 0U;
        auto channel_it = it.begin();
        while(channel_it != it.end()) {
            ++channel_count;
            ++channel_it;
        }
        ASSERT_EQ(static_cast<std::size_t>(number_of_channels), channel_count);
    }
}

/**
* @test CommonTimeFrequencyTester::test_channel_frequencies_start_end generic tester for all the TF and FT data types.
* @given Input (TF or FT) data types
* @when when the channel frequencies are set
* @then test will check is the values are set properly and fails if they are not.
*/

ALGORITHM_TYPED_TEST_P(CommonTimeFrequencyTester, test_channel_frequencies_start_end)
{
    (void) device;
    data::DimensionSize<data::Time> number_of_spectra(1U);
    data::DimensionSize<data::Frequency> number_of_channels(0);
    typedef typename TypeParam::DataType DataType;

    for(; number_of_channels < data::DimensionSize<data::Frequency>(5U); ++number_of_channels) {
        DataType tf(number_of_spectra, number_of_channels);
        auto f1 =  5.0 * boost::units::si::mega * boost::units::si::hertz;
        auto delta =  1.0 * boost::units::si::mega * boost::units::si::hertz;
        auto f2 =  12.0 * delta - f1;

        tf.set_channel_frequencies_const_width( f1, delta );
        {   // USe Case: first fre < sencond
            auto generated = tf.channel_frequencies();
            ASSERT_EQ(generated.size(), number_of_channels);

            for(std::size_t i = 0U; i < number_of_channels; ++i) {
                auto dif = (double)i * delta;
                ASSERT_TRUE( f1 + dif == generated[i]) << "element " << i << " dif=" << dif << " generated=" << generated[i];
            }
        }
        tf.set_channel_frequencies_const_width( f2, -1.0 * delta );
        {
            // Use Case: first freq > sencond
            auto generated = tf.channel_frequencies();
            ASSERT_EQ(generated.size(), number_of_channels);
            for(std::size_t i = 0U; i < number_of_channels; ++i) {
                ASSERT_TRUE( f2 - (double)i * delta == generated[i]) << "element " << i;
            }
        }
    }
}

/**
* @test CommonTimeFrequencyTester::test_low_high_freq generic tester for all the TF and FT data types.
* @given Input (TF or FT) data types
* @when when the channel frequencies are set.
* @then test will check if the channel ordering is set properly.
*/
ALGORITHM_TYPED_TEST_P(CommonTimeFrequencyTester, test_low_high_freq)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    data::DimensionSize<data::Time> number_of_spectra(4U);
    data::DimensionSize<data::Frequency> number_of_channels(10U);

    DataType tf(number_of_spectra, number_of_channels);
    typedef std::pair<typename DataType::FrequencyType, typename DataType::FrequencyType> ReturnType;

    // eexpect 0 values if channel_freq are not set
    ReturnType r = tf.low_high_frequencies();
    ASSERT_EQ(0.0, r.first.value());
    ASSERT_EQ(0.0, r.second.value());

    auto f1 =  typename DataType::FrequencyType(10.0 * boost::units::si::hertz);
    auto f2 =  typename DataType::FrequencyType(20.0 * boost::units::si::hertz);
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);

    // low to haigh channel freq ordering
    tf.set_channel_frequencies_const_width( f1, delta );
    ASSERT_EQ(tf.channel_frequencies().size(), number_of_channels);
    r = tf.low_high_frequencies();
    ASSERT_EQ(f1, r.first);
    ASSERT_EQ(f2, r.second);

    // high to low tfreq ordering
    tf.set_channel_frequencies_const_width( f2, -1.0 *delta );
    ASSERT_EQ(tf.channel_frequencies().size(), number_of_channels);
    r = tf.low_high_frequencies();
    ASSERT_EQ(f1, r.first);
    ASSERT_EQ(f2, r.second);

}

/**
* @test CommonTimeFrequencyTester::set_channel generic tester for all the TF and FT data types.
* @given Input (TF or FT) data types
* @when when the channel frequencies are set.
* @then test will check if set_channel() method does its job.
*/
ALGORITHM_TYPED_TEST_P(CommonTimeFrequencyTester, set_channel)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;

    data::DimensionSize<data::Time> number_of_spectra(3U);
    data::DimensionSize<data::Frequency> number_of_channels(5U);

    for(unsigned set_channel=0; set_channel<number_of_channels; ++set_channel)
    {
        SCOPED_TRACE(set_channel);
        DataType tf(number_of_spectra, number_of_channels);
        ASSERT_EQ(tf.number_of_channels(), number_of_channels);
        ASSERT_EQ(tf.number_of_spectra(), number_of_spectra);
        //std::fill(tf.begin(), tf.end(), 0.0);
        tf.set_channel(set_channel, 1.0);
        for(unsigned channel_num=0; channel_num<number_of_channels; ++channel_num) {
            SCOPED_TRACE(channel_num);
            auto channel=tf.channel(channel_num);
            auto channel_it = channel.begin();
            ASSERT_EQ(channel.template dimension<data::Time>(), number_of_spectra);
            ASSERT_EQ(channel.template dimension<data::Frequency>(), DimensionSize<data::Frequency>(1));
            unsigned sample_count=0U;
            while(channel_it != channel.end()) {
                ++sample_count;
                if(channel_num != set_channel) {
                    ASSERT_EQ(0.0, *channel_it);
                }
                else {
                    ASSERT_EQ(1.0, *channel_it);
                }
                ++channel_it;
            }
            ASSERT_EQ(static_cast<std::size_t>(number_of_spectra), sample_count);
        }
    }
}

/**
* @test CommonTimeFrequencyTester::test_start_time_offset generic tester for all the TF and FT data types.
* @given Input (TF or FT) data types
* @when when the channel frequencies and start time are set.
* @then test will check if start_time() is set properly.
*/
ALGORITHM_TYPED_TEST_P(CommonTimeFrequencyTester, test_start_time_offset)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;

    data::DimensionSize<data::Time> number_of_spectra(100U);
    data::DimensionSize<data::Frequency> number_of_channels(1U);

    DataType tf(number_of_spectra, number_of_channels);
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    typename DataType::TimeType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    tf.sample_interval(interval);
    tf.start_time(epoch);
    epoch += std::chrono::duration<double>(tf.sample_interval().value()*std::size_t(12));
    ASSERT_EQ(tf.start_time(12L), epoch);
}

// each test defined by ALGORITHM_TYPED_TEST_P must be added to the
REGISTER_TYPED_TEST_SUITE_P(CommonTimeFrequencyTester, test_dimensions, test_samples, test_channel_frequencies_start_end, test_low_high_freq, set_channel, test_start_time_offset);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska