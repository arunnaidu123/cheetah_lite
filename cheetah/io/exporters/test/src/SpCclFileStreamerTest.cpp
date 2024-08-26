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
#include "cheetah/io/exporters/test/SpCclFileStreamerTest.h"
#include "cheetah/io/exporters/SpCclFileStreamer.h"
#include "cheetah/utils/JulianClock.h"
#include "panda/test/TestDir.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


SpCclFileStreamerTest::SpCclFileStreamerTest()
    : ::testing::Test()
{
}

SpCclFileStreamerTest::~SpCclFileStreamerTest()
{
}

void SpCclFileStreamerTest::SetUp()
{
}

void SpCclFileStreamerTest::TearDown()
{
}

TEST_F(SpCclFileStreamerTest, test_write)
{
    // create a tmp directoy name
    typedef typename data::SpCcl<uint8_t>::TimeFrequencyType TimeFrequencyType;

    panda::test::TestDir tmp_dir;
    SpCclFileStreamerConfig config;
    ASSERT_NO_THROW(tmp_dir.create());
    config.dir(tmp_dir.dir_name());
    {
        typename utils::JulianClock::time_point start_time(utils::julian_day(2458179.500000));
        data::SpCcl<uint8_t>::BlocksType blocks;
        std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(1000)
                                                                            ,data::DimensionSize<data::Frequency>(10));
        block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));
        blocks.push_back(block);
        blocks.push_back(std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(10)));
        data::SpCcl<uint8_t> d1(blocks, data::DimensionIndex<data::Time>(0));
        typename data::SpCcl<uint8_t>::SpCandidateType::Dm dm(12.0 * data::parsecs_per_cube_cm);
        //set the single pulse candidate width to 1.24 ms
        data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
        //set the single pulse start time to 2.23 seconds
        data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
        //set the candidate significance
        float sigma = 20.0;

        for (auto idx=0; idx<10; ++idx)
        {
            data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
            d1.push_back(candidate);
        }

        exporters::SpCclFileStreamer<uint8_t> writer(config);
        writer << d1;
    } // should flush buffers to OS on leaving scope
    sync(); // ensure the OS actually writes to disc
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    ASSERT_FALSE(it == boost::filesystem::directory_iterator());
    boost::filesystem::path file = *it;
    ASSERT_EQ(file.extension().string(), ".spccl");
    ASSERT_EQ(file.stem().string(), "2018_03_02_00:00:00");
    ASSERT_TRUE(boost::filesystem::exists(it->path()));
    ASSERT_EQ(1U, std::distance(it, boost::filesystem::directory_iterator()));
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
