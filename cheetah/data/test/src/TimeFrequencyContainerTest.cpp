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
#include "cheetah/data/test/TimeFrequencyContainerTest.h"
#include "cheetah/data/TimeFrequencyContainer.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {


TimeFrequencyContainerTest::TimeFrequencyContainerTest()
    : ::testing::Test()
{
}

TimeFrequencyContainerTest::~TimeFrequencyContainerTest()
{
}

void TimeFrequencyContainerTest::SetUp()
{
}

void TimeFrequencyContainerTest::TearDown()
{
}

TEST_F(TimeFrequencyContainerTest, test_unequal_channels)
{
    typedef TimeFrequency<Cpu, float> TimeFrequencyType;
    data::DimensionSize<data::Frequency> number_of_channels(1000);
    TimeFrequencyType::TimeType interval(10 * data::milliseconds);
    data::DimensionSize<data::Time> n_samples(100);
    std::shared_ptr<TimeFrequencyType> time_freq_1=std::make_shared<TimeFrequencyType>(n_samples, number_of_channels);
    time_freq_1->sample_interval(interval);
    time_freq_1->start_time(TimeFrequencyType::TimePointType(std::chrono::seconds(10)));
    std::shared_ptr<TimeFrequencyType> time_freq_2=std::make_shared<TimeFrequencyType>(n_samples, number_of_channels - 1);
    time_freq_2->sample_interval(interval);
    time_freq_2->start_time(time_freq_1->end_time() + interval);

    TimeFrequencyContainer<Cpu, float> tfs;
    tfs.add(time_freq_1);
    ASSERT_THROW(tfs.add(time_freq_2), panda::Error);
}

TEST_F(TimeFrequencyContainerTest, test_iterator)
{
    typedef TimeFrequency<Cpu, float> TimeFrequencyType;
    DimensionSize<Frequency> number_of_channels(1000);
    TimeFrequencyType::TimeType interval(10 * data::milliseconds);
    std::shared_ptr<TimeFrequencyType> time_freq_1=std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(100), number_of_channels);
    time_freq_1->sample_interval(interval);
    time_freq_1->start_time(TimeFrequencyType::TimePointType(std::chrono::seconds(10)));
    ASSERT_EQ(time_freq_1->end_time(), time_freq_1->start_time() + 99 * interval);
    std::shared_ptr<TimeFrequencyType> time_freq_2=std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(500), number_of_channels);
    time_freq_2->sample_interval(interval);
    time_freq_2->start_time(time_freq_1->end_time() + interval);
    std::shared_ptr<TimeFrequencyType> time_freq_3=std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(1000), number_of_channels);
    time_freq_3->sample_interval(interval);
    time_freq_3->start_time(time_freq_2->end_time() + interval + interval);

    TimeFrequencyContainer<Cpu, float> tfs;
    tfs.add(time_freq_1);
    tfs.add(*time_freq_2);
    tfs.add(*time_freq_3);

    std::size_t channel_count=0;
    std::size_t td_count=0;
    std::vector<TimeFrequencyType::TimePointType> time_points;
    auto channel_end_handler = [&]() { ++channel_count; };
    auto time_discontinuity_handler = [&](TimeFrequencyType::TimePointType const& tp, typename TimeFrequencyType::TimeType const&) { 
        ++td_count;
        time_points.push_back(tp);
    };
    auto it = tfs.begin(channel_end_handler, time_discontinuity_handler);
    auto end = tfs.end();
    std::size_t count=0;
    while(it!=end) {
        ++count;
        float val = *it;
        ++val; // use so we don't get a warning msg
        static_assert(std::is_same<decltype(*it), const float&>::value, "unexpected type from iterator dereference");
        ++it;
    }
    ASSERT_EQ(time_freq_1->data_size() + time_freq_2->data_size() + time_freq_3->data_size(), count);
    ASSERT_EQ(channel_count, time_freq_1->number_of_spectra() + time_freq_2->number_of_spectra() + time_freq_3->number_of_spectra());
    ASSERT_EQ(2U, td_count);
    ASSERT_EQ(time_points.size(), td_count);
    ASSERT_EQ(time_freq_1->start_time(), time_points[0]);
    ASSERT_EQ(time_freq_3->start_time(), time_points[1]);
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
