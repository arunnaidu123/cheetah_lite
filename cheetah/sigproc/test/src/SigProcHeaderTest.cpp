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
#include "cheetah/sigproc/test/SigProcHeaderTest.h"
#include "cheetah/sigproc/SigProcHeader.h"
#include "cheetah/utils/chrono.h"
#include "cheetah/data/Units.h"

#include <sstream>

namespace ska {
namespace cheetah {
namespace sigproc {
namespace test {


SigProcHeaderTest::SigProcHeaderTest()
    : ::testing::Test()
{
}

SigProcHeaderTest::~SigProcHeaderTest()
{
}

void SigProcHeaderTest::SetUp()
{
}

void SigProcHeaderTest::TearDown()
{
}

// save a header to a stream and then restore it into a fresh SigProcHeader object which is returned
SigProcHeader save_restore(SigProcHeader const& h)
{
    std::stringstream ss;
    h.write(ss); 
    SigProcHeader r;
    r.read(ss);
    return r;
}

TEST_F(SigProcHeaderTest, number_of_channels)
{
    SigProcHeader h;
    ASSERT_EQ(0, h.number_of_channels());
    h.number_of_channels(332);
    ASSERT_EQ(332, h.number_of_channels());
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.number_of_channels(), h.number_of_channels());

    h.reset();
    ASSERT_EQ(0, h.number_of_channels());
}

TEST_F(SigProcHeaderTest, number_of_bits)
{
    SigProcHeader h;
    ASSERT_EQ(0, h.number_of_bits());
    h.number_of_bits(32);
    ASSERT_EQ(32, h.number_of_bits());
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.number_of_bits(), h.number_of_bits());
    h.reset();
    ASSERT_EQ(0, h.number_of_bits());
}

TEST_F(SigProcHeaderTest, test_start_time)
{
    utils::TimePoint<utils::ModifiedJulianClock> epoch(utils::julian_day(0));
    utils::TimePoint<utils::ModifiedJulianClock> t(utils::julian_day(12));
    SigProcHeader h;
    ASSERT_EQ(epoch, h.start_time());
    h.start_time(t);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.start_time(), h.start_time());
    ASSERT_EQ(t, h.start_time());
    h.reset();
    ASSERT_EQ(epoch, h.start_time());
}

TEST_F(SigProcHeaderTest, test_sample_interval)
{
    SigProcHeader::TimeType zero = SigProcHeader::TimeType::from_value(0.0);
    SigProcHeader::TimeType t = SigProcHeader::TimeType::from_value(20.0);
    SigProcHeader h;
    ASSERT_EQ(zero, h.sample_interval());
    h.sample_interval(t);
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.sample_interval(), h.sample_interval());
    ASSERT_EQ(t, h.sample_interval());
    h.reset();
    ASSERT_EQ(zero, h.sample_interval());
}

TEST_F(SigProcHeaderTest, test_source_name)
{
    std::string s("abc");
    std::string empty;

    SigProcHeader h;
    ASSERT_FALSE(h.source_name().is_set());
    h.source_name(s);
    ASSERT_EQ(s, *h.source_name());
    SigProcHeader h2 = save_restore(h);
    ASSERT_EQ(h2.source_name(), h.source_name());
    h.reset();
    ASSERT_FALSE(h.source_name().is_set());
}

TEST_F(SigProcHeaderTest, test_size)
{
    SigProcHeader h;
    std::stringstream ss;
    h.write(ss); 

    SigProcHeader r;
    r.read(ss);
    
    ASSERT_EQ(r.size(), ss.str().size());
    ASSERT_EQ(r.size(), h.size());
}

TEST_F(SigProcHeaderTest, test_frequency_array)
{
    typedef typename SigProcHeader::FrequencyType FrequencyType;
    std::vector<FrequencyType> channel_frequencies { FrequencyType(1.5 * data::hz), FrequencyType( 2.0 * data::hz) };

    SigProcHeader h;
    h.frequency_channels(channel_frequencies);
    ASSERT_EQ(h.frequency_channels(), channel_frequencies);

    std::stringstream ss;
    h.write(ss); 

    SigProcHeader r;
    r.read(ss);
    
    ASSERT_EQ(h.frequency_channels(), r.frequency_channels());
    ASSERT_EQ(channel_frequencies.size(), (unsigned)h.number_of_channels());
    ASSERT_EQ(channel_frequencies.size(), (unsigned)r.number_of_channels());

    h.reset();
    ASSERT_EQ(0U, h.frequency_channels().size());
}

} // namespace test
} // namespace sigproc
} // namespace cheetah
} // namespace ska
