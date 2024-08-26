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
#include "cheetah/io/exporters/test/SpCandidateDataStreamerTest.h"
#include "cheetah/io/exporters//SpCandidateDataStreamer.h"
#include "cheetah/utils/JulianClock.h"
#include "panda/test/TestDir.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


template<typename TimeFrequencyType>
SpCandidateDataStreamerTest<TimeFrequencyType>::SpCandidateDataStreamerTest()
    : ::testing::Test()
{
}

TYPED_TEST(SpCandidateDataStreamerTest, test_write)
{
    typedef TypeParam TimeFrequencyType;
    typedef data::SpCandidateData<TimeFrequencyType> DataType;
    typedef typename DataType::CandidateType CandidateType;
    typedef typename CandidateType::Dm Dm;
    typedef typename CandidateType::MsecTimeType MsecTimeType;
    typedef typename CandidateType::Duration Duration;

    typename utils::ModifiedJulianClock::time_point start_time(utils::julian_day(58179.0000000));

    // create some candidates
    CandidateType cand1( Dm(12.0 * data::parsecs_per_cube_cm)
                       , start_time
                       , MsecTimeType(0.001 * boost::units::si::seconds)
                       , Duration(0.003 * boost::units::si::seconds)
                       , 2.0
                        );


    SpCandidateDataStreamerConfig config;

    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());
    config.dir(tmp_dir.dir_name());

    {
        // create some data to write
        std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(1000)
                                                                        ,data::DimensionSize<data::Frequency>(10));
        block->start_time(start_time);
        DataType data(block);

        // Add some condidates
        for(unsigned i=0; i<3; ++i) {
            data.add(CandidateType(cand1));
        }


        exporters::SpCandidateDataStreamer<TimeFrequencyType> writer(config);
        writer << data;
    } // destructor should be called here, flushing buffers etc
    sync(); // ensure the OS actually writes to disc
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    ASSERT_FALSE(it == boost::filesystem::directory_iterator());
    boost::filesystem::path file = *it;
    ASSERT_EQ(file.extension().string(), ".spccl");
    ASSERT_EQ(file.stem().string(), "2018_03_02_00:00:00");
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
