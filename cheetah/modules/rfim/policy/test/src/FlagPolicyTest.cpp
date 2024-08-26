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
#include "cheetah/modules/rfim/policy/test/FlagPolicyTest.h"
#include "cheetah/modules/rfim/policy/FlagPolicy.h"
#include "panda/CharAsInt.h"
#include <numeric>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {


FlagPolicyTest::FlagPolicyTest()
    : ::testing::Test()
{
}

FlagPolicyTest::~FlagPolicyTest()
{
}

void FlagPolicyTest::SetUp()
{
}

void FlagPolicyTest::TearDown()
{
}

TEST_F(FlagPolicyTest, test_no_marks)
{
    // Use case: contructed and data extracted, but no bad marks are made
    typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;
    typedef FlagPolicy<TimeFrequencyType> FlagPolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);

    const FlagPolicyType policy;
    auto adapter = policy.adapter(*tf);
    std::shared_ptr<data::RfimFlaggedData<TimeFrequencyType>> data=adapter.data();
    TimeFrequencyType& output_data = static_cast<TimeFrequencyType&>(*data);
    ASSERT_EQ(&output_data, tf.get()); // make sure we get the same data out we put in

    // check all is marked good
    for(auto const sample : data->rfi_flags())
    {
        ASSERT_EQ(false, (bool)sample);
    }
}

TEST_F(FlagPolicyTest, test_const_tf_data)
{
    // Use case: adapter contructed with a const TF data block
    typedef const data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;
    typedef FlagPolicy<TimeFrequencyType> FlagPolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);

    const FlagPolicyType policy;
    auto adapter = policy.adapter(*tf);
    std::shared_ptr<data::RfimFlaggedData<TimeFrequencyType>> data=adapter.data();
    TimeFrequencyType const& output_data = static_cast<TimeFrequencyType const&>(*data);
    ASSERT_EQ(&output_data, tf.get()); // make sure we get the same data out we put in
}

TEST_F(FlagPolicyTest, test_mark_bad_channels)
{
    typedef uint8_t NumericalType;
    typedef const data::TimeFrequency<Cpu, NumericalType> TimeFrequencyType;
    typedef FlagPolicy<TimeFrequencyType> FlagPolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);
    TimeFrequencyType tf_copy = *tf;

    const FlagPolicyType policy;
    auto adapter = policy.adapter(*tf);

    // Usw cases - bad channels
    for(std::size_t channel_number=0; channel_number < tf->dimension<data::Frequency>(); ++channel_number) {
        adapter.mark_bad(tf->channel(channel_number));
    }

    // verify the contents of the data has not been changed
    auto tf_copy_it=tf_copy.begin();
    for(auto const& s : *tf) {
        ASSERT_EQ(static_cast<typename panda::CharAsInt<NumericalType>::type>(*tf_copy_it), static_cast<typename panda::CharAsInt<NumericalType>::type>(s));
        ++tf_copy_it;
    }
}

TEST_F(FlagPolicyTest, test_mark_bad_spectra)
{
    typedef uint8_t NumericalType;
    typedef data::TimeFrequency<Cpu, NumericalType> TimeFrequencyType;
    typedef FlagPolicy<TimeFrequencyType> FlagPolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);
    std::iota(tf->begin(), tf->end(), 0);

    const FlagPolicyType policy;
    auto adapter = policy.adapter(*tf);

    // Usw cases - bad channels
    for(std::size_t spectrum_number=0; spectrum_number < tf->dimension<data::Time>(); ++spectrum_number) {
        adapter.mark_bad(tf->spectrum(spectrum_number));
    }

    // verify the contents of the data has not been changed
    typename panda::CharAsInt<NumericalType>::type i=0;
    for(auto const& s : *tf) {
        ASSERT_EQ(i, static_cast<typename panda::CharAsInt<NumericalType>::type>(s));
        ++i;
    }
}

TEST_F(FlagPolicyTest, test_mark_only_bad_channels)
{
    typedef uint8_t NumericalType;
    typedef data::TimeFrequency<Cpu, NumericalType> TimeFrequencyType;
    typedef FlagPolicy<TimeFrequencyType> PolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);
    std::iota(tf->begin(), tf->end(), 0);

    PolicyType policy;
    auto adapter = policy.adapter(*tf);
    for(data::DimensionIndex<data::Frequency> channel_number(0); channel_number < tf->dimension<data::Frequency>(); ++channel_number) {
        adapter.mark_bad(tf->channel(channel_number));
    }

    // verify the contents of the data has not been changed
    typename panda::CharAsInt<NumericalType>::type i=0;
    for(auto const& s : *tf) {
        ASSERT_EQ(i, static_cast<typename panda::CharAsInt<NumericalType>::type>(s));
        ++i;
    }

    std::shared_ptr<data::RfimFlaggedData<TimeFrequencyType>> data=adapter.data();
    for(auto const& flag : data->rfi_flags()) {
        ASSERT_TRUE(flag);
    }
}

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
