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
#include "cheetah/modules/ddtr/test/BufferingTest.h"
#include "cheetah/modules/ddtr/detail/Buffering.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


BufferingTest::BufferingTest()
    : ::testing::Test()
{
}

BufferingTest::~BufferingTest()
{
}

void BufferingTest::SetUp()
{
}

void BufferingTest::TearDown()
{
}

template<template <typename> class AggregationBufferFactoryTemplate=TimeFrequencyBufferFactory>
class TestDdtrTraits
{
    public:
        typedef BufferingTest::TimeFrequencyType TimeFrequencyType;
        typedef ska::panda::AggregationBufferFiller<TimeFrequencyType, AggregationBufferFactoryTemplate<TimeFrequencyType>> BufferFillerType;
        typedef typename BufferFillerType::AggregationBufferType BufferType;
};

class TestDedispersionPlan
{
    public:
        typedef BufferingTest::TimeFrequencyType TimeFrequencyType;

    public:

        data::DimensionSize<data::Time> buffer_overlap()
        {
            return data::DimensionSize<data::Time>(16);
        }

        data::DimensionSize<data::Time> dedispersion_strategy(TimeFrequencyType const&)
        {
            return (data::DimensionSize<data::Time>(1024));
        }
};

TEST_F(BufferingTest, test_buffer_handler_call)
{
    TestDedispersionPlan plan;
    unsigned int handler_calls=0;
    Buffering<TestDdtrTraits<TimeFrequencyBufferFactory>, TestDedispersionPlan> tbuffer([&](typename TestDdtrTraits<>::BufferType&& buffer)
        {
            if(buffer.composition().empty())
            {
                PANDA_LOG_WARN << "received an empty buffer";
                return;
            }
            handler_calls += 1;
        }
        , plan
        , 1024 // buffer size
        );

    std::shared_ptr<TimeFrequencyType> data = TimeFrequencyType::make_shared(pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(1500),
                                          pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(10));
    data->sample_interval(typename TimeFrequencyType::TimeType(50.0 * boost::units::si::milli * boost::units::si::seconds));
    data->set_channel_frequencies_const_width(data::FrequencyType(1000.0 * boost::units::si::mega * data::hz), data::FrequencyType(-30.0 * boost::units::si::mega * data::hz));
    tbuffer(*data);
    ASSERT_EQ(plan.buffer_overlap(), data::DimensionSize<data::Time>(16));
    ASSERT_EQ(handler_calls,1);
}

TEST_F(BufferingTest, test_buffer_overlap)
{
    TestDedispersionPlan plan;
    unsigned int handler_calls=0;
    Buffering<TestDdtrTraits<TimeFrequencyBufferFactory>, TestDedispersionPlan> tbuffer([&](typename TestDdtrTraits<>::BufferType&& buffer)
        {
            if(buffer.composition().empty())
            {
                PANDA_LOG_WARN << "received an empty buffer";
                return;
            }
            handler_calls += 1;

        }
        , plan
        , 1024 // buffer size
        );

    std::shared_ptr<TimeFrequencyType> data = TimeFrequencyType::make_shared(pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(1024),
                                          pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(10));
    data->sample_interval(typename TimeFrequencyType::TimeType(50.0 * boost::units::si::milli * boost::units::si::seconds));
    data->set_channel_frequencies_const_width(data::FrequencyType(1000.0 * boost::units::si::mega * data::hz), data::FrequencyType(-30.0 * boost::units::si::mega * data::hz));

    tbuffer(*data);
    ASSERT_EQ(handler_calls,1);
    std::shared_ptr<TimeFrequencyType> data1 = TimeFrequencyType::make_shared(pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(1008),
                                          pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(10));
    data1->sample_interval(typename TimeFrequencyType::TimeType(50.0 * boost::units::si::milli * boost::units::si::seconds));
    data1->set_channel_frequencies_const_width(data::FrequencyType(1000.0 * boost::units::si::mega * data::hz), data::FrequencyType(-30.0 * boost::units::si::mega * data::hz));

    tbuffer(*data1);
    ASSERT_EQ(handler_calls,2);
}

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
