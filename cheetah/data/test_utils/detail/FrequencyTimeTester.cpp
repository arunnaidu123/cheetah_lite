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

namespace ska {
namespace cheetah {
namespace data {
namespace test {


template <typename TestTraits>
FrequencyTimeTester<TestTraits>::FrequencyTimeTester()
{
}

template <typename TestTraits>
FrequencyTimeTester<TestTraits>::~FrequencyTimeTester()
{
}

template <typename TestTraits>
void FrequencyTimeTester<TestTraits>::SetUp()
{
}

template <typename TestTraits>
void FrequencyTimeTester<TestTraits>::TearDown()
{
}

ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, test_dimensions)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    data::DimensionSize<data::Time> number_of_spectra(10);
    data::DimensionSize<data::Frequency> number_of_channels(10);
    DataType ft_1(number_of_channels, number_of_spectra);
    DataType ft_2(number_of_spectra, number_of_channels);

    ASSERT_EQ(ft_1.number_of_channels(), number_of_channels);
    ASSERT_EQ(ft_2.number_of_channels(), number_of_channels);
    ASSERT_EQ(ft_1.number_of_spectra(), number_of_spectra);
    ASSERT_EQ(ft_2.number_of_spectra(), number_of_spectra);
}

ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, test_samples)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    data::DimensionSize<data::Time> number_of_spectra(10U);
    data::DimensionSize<data::Frequency> number_of_channels(5U);

    DataType ft(number_of_spectra, number_of_channels);
    ASSERT_EQ(static_cast<std::size_t>(number_of_spectra), ft.number_of_spectra());
    ASSERT_EQ(static_cast<std::size_t>(number_of_channels), ft.number_of_channels());

    for(unsigned sample_number=0; sample_number < ft.number_of_spectra(); ++sample_number) {
        SCOPED_TRACE("sample = " + std::to_string(sample_number));
        typename DataType::Spectra it = ft.spectrum(sample_number);
        unsigned channel_count = 0U;
        auto channel_it = it.begin();
        while(channel_it != it.end()) {
            ++channel_count;
            ++channel_it;
        }
        ASSERT_EQ(static_cast<std::size_t>(number_of_channels), channel_count);
    }
}

ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, test_channel_frequencies_start_end)
{
    (void) device;
    data::DimensionSize<data::Time> number_of_spectra(1U);
    data::DimensionSize<data::Frequency> number_of_channels(0);
    typedef typename TypeParam::DataType DataType;

    for(; number_of_channels < data::DimensionSize<data::Frequency>(5U); ++number_of_channels) {
        DataType ft(number_of_spectra, number_of_channels);
        auto f1 =  5.0 * boost::units::si::mega * boost::units::si::hertz;
        auto delta =  1.0 * boost::units::si::mega * boost::units::si::hertz;
        auto f2 =  12.0 * delta - f1;

        ft.set_channel_frequencies_const_width( f1, delta );
        {   // USe Case: first fre < sencond
            auto generated = ft.channel_frequencies();
            ASSERT_EQ(generated.size(), number_of_channels);

            for(std::size_t i = 0U; i < number_of_channels; ++i) {
                auto dif = (double)i * delta;
                ASSERT_TRUE( f1 + dif == generated[i]) << "element " << i << " dif=" << dif << " generated=" << generated[i];
            }
        }
        ft.set_channel_frequencies_const_width( f2, -1.0 * delta );
        {
            // Use Case: first freq > sencond
            auto generated = ft.channel_frequencies();
            ASSERT_EQ(generated.size(), number_of_channels);
            for(std::size_t i = 0U; i < number_of_channels; ++i) {
                ASSERT_TRUE( f2 - (double)i * delta == generated[i]) << "element " << i;
            }
        }
    }
}

ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, test_low_high_freq)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    data::DimensionSize<data::Time> number_of_spectra(4U);
    data::DimensionSize<data::Frequency> number_of_channels(10U);

    DataType ft(number_of_spectra, number_of_channels);
    typedef std::pair<typename DataType::FrequencyType, typename DataType::FrequencyType> ReturnType;

    // eexpect 0 values if channel_freq are not set
    ReturnType r = ft.low_high_frequencies();
    ASSERT_EQ(0.0, r.first.value());
    ASSERT_EQ(0.0, r.second.value());

    auto f1 =  typename DataType::FrequencyType(10.0 * boost::units::si::hertz);
    auto f2 =  typename DataType::FrequencyType(20.0 * boost::units::si::hertz);
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);

    // low to haigh channel freq ordering
    ft.set_channel_frequencies_const_width( f1, delta );
    ASSERT_EQ(ft.channel_frequencies().size(), number_of_channels);
    r = ft.low_high_frequencies();
    ASSERT_EQ(f1, r.first);
    ASSERT_EQ(f2, r.second);

    // high to low freq ordering
    ft.set_channel_frequencies_const_width( f2, -1.0 *delta );
    ASSERT_EQ(ft.channel_frequencies().size(), number_of_channels);
    r = ft.low_high_frequencies();
    ASSERT_EQ(f1, r.first);
    ASSERT_EQ(f2, r.second);

}

ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, set_channel)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;

    data::DimensionSize<data::Time> number_of_spectra(3U);
    data::DimensionSize<data::Frequency> number_of_channels(5U);

    for(unsigned set_channel=0; set_channel<number_of_channels; ++set_channel)
    {
        SCOPED_TRACE(set_channel);
        DataType ft(number_of_spectra, number_of_channels);
        ASSERT_EQ(ft.number_of_channels(), number_of_channels);
        ASSERT_EQ(ft.number_of_spectra(), number_of_spectra);
        //std::fill(ft.begin(), ft.end(), 0.0);
        ft.set_channel(set_channel, 1.0);
        for(unsigned channel_num=0; channel_num<number_of_channels; ++channel_num) {
            SCOPED_TRACE(channel_num);
            auto channel=ft.channel(channel_num);
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

ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, test_start_time_offset)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;

    data::DimensionSize<data::Time> number_of_spectra(100U);
    data::DimensionSize<data::Frequency> number_of_channels(1U);

    DataType ft(number_of_spectra, number_of_channels);
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    typename DataType::TimeType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    ft.sample_interval(interval);
    ft.start_time(epoch);
    epoch += std::chrono::duration<double>(ft.sample_interval().value()*std::size_t(12));
    ASSERT_EQ(ft.start_time(12L), epoch);
}


ALGORITHM_TYPED_TEST_P(FrequencyTimeTester, test_transpose)
{
    (void) device;
    typedef typename TypeParam::DataType DataType;
    typedef data::TimeFrequency<Cpu, typename DataType::NumericalRep> TfDataType;
    typedef data::FrequencyTime<Cpu, typename DataType::NumericalRep> FtDataType;
    data::DimensionSize<data::Time> number_of_spectra(1024U);
    data::DimensionSize<data::Frequency> number_of_channels(512U);

    TfDataType tf(number_of_spectra, number_of_channels);

    unsigned val=0;
    std::generate(tf.begin(), tf.end(), [&]() { return val++; });

    auto f1 =  typename data::TimeFrequency<Cpu, typename DataType::NumericalRep>::FrequencyType(100.0 * boost::units::si::hertz);
    auto f2 =  typename data::TimeFrequency<Cpu, typename DataType::NumericalRep>::FrequencyType(200.0 * boost::units::si::hertz);
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);

    tf.set_channel_frequencies_const_width( f1, delta);
    tf.start_time(epoch);

    FtDataType ft(tf);

    ASSERT_EQ(tf.number_of_channels(), ft.number_of_channels());
    ASSERT_EQ(tf.number_of_spectra(), ft.number_of_spectra());
    ASSERT_EQ(tf.number_of_spectra(), ft.number_of_spectra());
    ASSERT_EQ(tf.start_time(), ft.start_time());

    for(std::size_t i = 0U; i < ft.channel_frequencies().size(); ++i) {
        ASSERT_EQ(ft.channel_frequencies()[i], ft.channel_frequencies()[i]);
    }

    for(unsigned i=0; i < tf.number_of_spectra(); ++i)
    {
        // iterate over channels
        typename TfDataType::Spectra tf_spectrum = tf.spectrum(i);
        auto tf_channel_it = tf_spectrum.begin();
        typename FtDataType::Spectra ft_spectrum = ft.spectrum(i);
        auto ft_channel_it = ft_spectrum.begin();
        while(tf_channel_it != tf_spectrum.end())
        {
            ASSERT_EQ(*tf_channel_it, *ft_channel_it);
            ++tf_channel_it;
            ++ft_channel_it;
        }
    }

}

// each test defined by ALGORITHM_TYPED_TEST_P must be added to the
REGISTER_TYPED_TEST_SUITE_P(FrequencyTimeTester, test_dimensions, test_samples, test_channel_frequencies_start_end, test_low_high_freq, set_channel, test_start_time_offset, test_transpose);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
