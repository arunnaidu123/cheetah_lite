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
#include "cheetah/modules/rfim/test/MetricsTest.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {


MetricsTest::MetricsTest()
    : ::testing::Test()
{
}

MetricsTest::~MetricsTest()
{
}

void MetricsTest::SetUp()
{
}

void MetricsTest::TearDown()
{
}

TEST_F(MetricsTest, test_empty)
{
    typedef rfim::Metrics::TimeFrequencyFlagsType Flags;
    Flags expected;
    expected.reset();
    Flags given;
    given.reset();
    rfim::Metrics m(expected, given);
    ASSERT_EQ(m.num_correct(), 0U);
    ASSERT_EQ(m.num_rfi(), 0U);
    ASSERT_EQ(m.num_false_positives(), 0U);
    ASSERT_EQ(m.num_false_negatives(), 0U);
}

TEST_F(MetricsTest, test_true_positive)
{
    typedef rfim::Metrics::TimeFrequencyFlagsType Flags;
    DimensionSize<data::Time> num_spectra(20);
    DimensionSize<data::Frequency> num_channels(10);
    Flags expected(num_spectra, num_channels);
    expected.reset();
    expected[DimensionIndex<data::Time>(2)][DimensionIndex<data::Frequency>(3)] = true;
    expected[DimensionIndex<data::Time>(4)][DimensionIndex<data::Frequency>(4)] = true;
    Flags given(num_spectra, num_channels);
    given.reset();
    given[DimensionIndex<data::Time>(2)][DimensionIndex<data::Frequency>(3)] = true;
    given[DimensionIndex<data::Time>(4)][DimensionIndex<data::Frequency>(4)] = true;

    rfim::Metrics m(expected, given);
    ASSERT_EQ(m.num_correct(), 2U);
    ASSERT_DOUBLE_EQ(m.correct_percentage(), 100.0);
    ASSERT_EQ(m.num_rfi(), 2U);
    ASSERT_EQ(m.num_false_positives(), 0U);
    ASSERT_DOUBLE_EQ(m.false_positives_percentage(), 0.0);
    ASSERT_EQ(m.num_false_negatives(), 0U);
    ASSERT_DOUBLE_EQ(m.false_negatives_percentage(), 0.0);
}

TEST_F(MetricsTest, test_false_positive_rfi_present)
{
    typedef rfim::Metrics::TimeFrequencyFlagsType Flags;
    DimensionSize<data::Time> num_spectra(20);
    DimensionSize<data::Frequency> num_channels(10);
    Flags expected(num_spectra, num_channels);
    expected.reset();
    expected[DimensionIndex<data::Time>(2)][DimensionIndex<data::Frequency>(3)] = true;
    expected[DimensionIndex<data::Time>(4)][DimensionIndex<data::Frequency>(4)] = true;
    Flags given(num_spectra, num_channels);
    given.reset();
    given[DimensionIndex<data::Time>(2)][DimensionIndex<data::Frequency>(6)] = true;
    given[DimensionIndex<data::Time>(4)][DimensionIndex<data::Frequency>(9)] = true;

    rfim::Metrics m(expected, given);
    ASSERT_EQ(m.num_correct(), 0U);
    ASSERT_DOUBLE_EQ(m.rfi_detected_percentage(), 0.0);
    ASSERT_EQ(m.num_rfi(), 2U);
    ASSERT_EQ(m.num_false_positives(), 2U);
    ASSERT_DOUBLE_EQ(m.false_positives_percentage(), (2.0/expected.data_size()) * 100.0);
    ASSERT_EQ(m.num_false_negatives(), 2U);
    ASSERT_DOUBLE_EQ(m.false_negatives_percentage(), (2.0/expected.data_size()) * 100.0);
    ASSERT_DOUBLE_EQ(m.correct_percentage(), 100.0*(((double)(num_channels * num_spectra) - 4)/(num_channels * num_spectra)));
}

TEST_F(MetricsTest, test_false_positive_no_rfi_present)
{
    typedef rfim::Metrics::TimeFrequencyFlagsType Flags;
    DimensionSize<data::Time> num_spectra(20);
    DimensionSize<data::Frequency> num_channels(10);
    Flags expected(num_spectra, num_channels);
    expected.reset();
    Flags given(num_spectra, num_channels);
    given.reset();
    given[DimensionIndex<data::Time>(2)][DimensionIndex<data::Frequency>(6)] = true;
    given[DimensionIndex<data::Time>(4)][DimensionIndex<data::Frequency>(9)] = true;

    rfim::Metrics m(expected, given);
    ASSERT_EQ(m.num_correct(), 0U);
    ASSERT_DOUBLE_EQ(m.rfi_detected_percentage(), 100.0);
    ASSERT_EQ(m.num_rfi(), 0U);
    ASSERT_EQ(m.num_false_positives(), 2U);
    ASSERT_DOUBLE_EQ(m.false_positives_percentage(), (2.0/expected.data_size()) * 100.0);
    ASSERT_EQ(m.num_false_negatives(), 0U);
    ASSERT_DOUBLE_EQ(m.false_negatives_percentage(), 0.0);
    ASSERT_DOUBLE_EQ(m.correct_percentage(), 100.0*(((double)(num_channels * num_spectra) - 2)/(num_channels * num_spectra)));
}

TEST_F(MetricsTest, test_no_detections_rfi_present)
{
    typedef rfim::Metrics::TimeFrequencyFlagsType Flags;
    DimensionSize<data::Time> num_spectra(20);
    DimensionSize<data::Frequency> num_channels(10);
    Flags expected(num_spectra, num_channels);
    expected.reset();
    expected[DimensionIndex<data::Time>(2)][DimensionIndex<data::Frequency>(3)] = true;
    expected[DimensionIndex<data::Time>(4)][DimensionIndex<data::Frequency>(4)] = true;
    Flags given(num_spectra, num_channels);
    given.reset();

    rfim::Metrics m(expected, given);
    ASSERT_EQ(m.num_correct(), 0U);
    ASSERT_DOUBLE_EQ(m.rfi_detected_percentage(), 0.0);
    ASSERT_EQ(m.num_rfi(), 2U);
    ASSERT_EQ(m.num_false_positives(), 0U);
    ASSERT_DOUBLE_EQ(m.false_positives_percentage(), (0.0/expected.data_size()) * 100.0);
    ASSERT_EQ(m.num_false_negatives(), 2U);
    ASSERT_DOUBLE_EQ(m.false_negatives_percentage(), (2.0/expected.data_size()) * 100.0);
    ASSERT_DOUBLE_EQ(m.correct_percentage(), 100.0*(((double)(num_channels * num_spectra) - 2)/(num_channels * num_spectra)));
}

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
