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
#include "cheetah/modules/rfim/policy/test/LastUnflaggedTest.h"
#include "cheetah/modules/rfim/policy/LastUnflagged.h"
#include "panda/CharAsInt.h"
#include <numeric>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {


LastUnflaggedTest::LastUnflaggedTest()
    : ::testing::Test()
{
}

LastUnflaggedTest::~LastUnflaggedTest()
{
}

void LastUnflaggedTest::SetUp()
{
}

void LastUnflaggedTest::TearDown()
{
}

TEST_F(LastUnflaggedTest, test_no_marks)
{
    typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;
    typedef LastUnflagged<TimeFrequencyType> PolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);
    std::iota(tf->begin(), tf->end(), 0);

    {
        PolicyType policy;
        auto adapter = policy.adapter(*tf);
    }

    // verify the contents of the data has not changed
    std::size_t count=0;
    for(auto const& s : *tf) {
        ASSERT_EQ(count, s);
        ++count;
    }
}

TEST_F(LastUnflaggedTest, test_mark_only_bad_spectra)
{
    typedef uint8_t NumericalType;
    typedef data::TimeFrequency<Cpu, NumericalType> TimeFrequencyType;
    typedef LastUnflagged<TimeFrequencyType> PolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);
    std::iota(tf->begin(), tf->end(), 0);

    PolicyType policy;
    auto adapter = policy.adapter(*tf);
    for(data::DimensionIndex<data::Time> spectrum_number(0); spectrum_number < tf->dimension<data::Time>(); ++spectrum_number) {
        adapter.mark_bad(tf->spectrum(spectrum_number));
    }

    // verify the contents of the data has been set to zero
    for(auto const& s : *tf) {
        ASSERT_EQ(0, static_cast<typename panda::CharAsInt<NumericalType>::type>(s));
    }
}

TEST_F(LastUnflaggedTest, test_mark_only_bad_channels)
{
    typedef uint8_t NumericalType;
    typedef data::TimeFrequency<Cpu, NumericalType> TimeFrequencyType;
    typedef LastUnflagged<TimeFrequencyType> PolicyType;

    data::DimensionSize<data::Frequency> number_of_channels(10);
    data::DimensionSize<data::Time> number_of_spectra(5);
    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_channels, number_of_spectra);
    std::iota(tf->begin(), tf->end(), 0);

    PolicyType policy;
    auto adapter = policy.adapter(*tf);
    for(data::DimensionIndex<data::Frequency> channel_number(0); channel_number < tf->dimension<data::Frequency>(); ++channel_number) {
        adapter.mark_bad(tf->channel(channel_number));
    }

    // verify the contents of the data has been set to zero
    for(auto const& s : *tf) {
        ASSERT_EQ(0, static_cast<typename panda::CharAsInt<NumericalType>::type>(s));
    }
}

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
