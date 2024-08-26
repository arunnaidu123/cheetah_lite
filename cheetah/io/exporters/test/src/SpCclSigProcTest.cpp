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
#include "cheetah/io/exporters/test/SpCclSigProcTest.h"
#include "cheetah/io/exporters/SpCclSigProc.h"
#include "cheetah/utils/JulianClock.h"
#include "panda/test/TestDir.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


SpCclSigProcTest::SpCclSigProcTest()
   : ::testing::Test()
{
}

SpCclSigProcTest::~SpCclSigProcTest()
{
}

void SpCclSigProcTest::SetUp()
{
}

void SpCclSigProcTest::TearDown()
{
}

TEST_F(SpCclSigProcTest,test_cand_write)
{
    // create a tmp directoy name
    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());
    typedef typename data::SpCcl<uint8_t>::TimeFrequencyType TimeFrequencyType;
    data::TimeFrequency<Cpu, uint8_t>::TimeType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    data::SpCcl<uint8_t>::BlocksType blocks;
    std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(data::DimensionSize<data::Time>(100000)
            ,data::DimensionSize<data::Frequency>(10));
    (*block).sample_interval(interval);
    //block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));
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
        candidate.duration_end(2000 * boost::units::si::milli * boost::units::si::seconds);
        d1.push_back(candidate);
        tstart  += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(4.5 * boost::units::si::seconds);
    }

    data::CandidateWindowConfig candconfig;

    data::CandidateWindow& window = candconfig.window();

    window.ms_before(150.5 * boost::units::si::milli * boost::units::si::seconds);
    window.ms_after(150.5 * boost::units::si::milli * boost::units::si::seconds);

    SpCclSigProcConfig sigprocconfig;
    sigprocconfig.window_config(candconfig);
    sigprocconfig.dir(tmp_dir.dir_name());

    SpCclSigProc<uint8_t> sigproc(sigprocconfig);
    ASSERT_NO_THROW(sigproc << d1);
    sync();
    boost::filesystem::directory_iterator it_end;
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    auto count = 0;
    ASSERT_FALSE(it == boost::filesystem::directory_iterator());
    while (it != it_end)
    {
        boost::filesystem::path file = *it;
        ASSERT_EQ(file.extension().string(), ".fil");
        it++;
        count ++;
    }

    // check if the number of files are equal to the number of candidates
    ASSERT_EQ(d1.size(),count);

}

TEST_F(SpCclSigProcTest, test_one_candidates_sigproc_file_spanning_many_blocks)
{
/**
 * @brief This test is to make sure that when there is a candidate that spans multiple TF blocks, it
 * should result in only one sigproc file
 */
    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());
    typedef data::SpCandidate<Cpu, float> SpCandidateType;
    typedef data::SpCcl<uint8_t>::BlocksType::value_type::element_type TimeFreqType;
    data::TimeFrequency<Cpu, uint8_t> tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));
    data::TimeFrequency<Cpu, uint8_t> tf2(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));
    data::TimeFrequency<Cpu, uint8_t> tf3(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));
    data::TimeFrequency<Cpu, uint8_t> tf4(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));
    data::TimeFrequency<Cpu, uint8_t> tf5(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));
    data::TimeFrequency<Cpu, uint8_t> tf6(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));


    tf1.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf2.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf3.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf4.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf5.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf6.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));


    data::SpCcl<uint8_t>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);
    std::fill(tf2.begin(),tf2.end(),1);
    std::fill(tf3.begin(),tf3.end(),2);
    std::fill(tf4.begin(),tf4.end(),0);
    std::fill(tf5.begin(),tf5.end(),1);
    std::fill(tf6.begin(),tf6.end(),2);


    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf1));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf2));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf3));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf4));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf5));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf6));


    // Generate SpCcl instance
    data::SpCcl<uint8_t> d1(tf_v, data::DimensionIndex<data::Time>(0));

    // Generate Candidates
    typename SpCandidateType::Dm dm(100.0 * data::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(1 * boost::units::si::milli * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(5 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;
    //set end time
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);
 // Generate Candidates
    typename SpCandidateType::Dm dm1(100.0 * data::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width1(1 * boost::units::si::milli * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart1(305 * boost::units::si::seconds);
    //set the candidate significance
    float sigma1 = 20.0;
    //set end time
    SpCandidateType::MsecTimeType tend1(2000 * boost::units::si::milli * boost::units::si::seconds);


    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    SpCandidateType candidate1(dm1, tstart1, width1, sigma1, 2);

    candidate.duration_end(tend);
    candidate1.duration_end(tend1);
    d1.push_back(candidate);
    d1.push_back(candidate1);

    data::CandidateWindowConfig candconfig;

    data::CandidateWindow& window = candconfig.window();

    window.ms_before(2000 * boost::units::si::milli * boost::units::si::seconds);
    window.ms_after(200000 * boost::units::si::milli * boost::units::si::seconds);

    SpCclSigProcConfig sigprocconfig;
    sigprocconfig.window_config(candconfig);
    sigprocconfig.dir(tmp_dir.dir_name());

    SpCclSigProc<uint8_t> sigproc(sigprocconfig);
    ASSERT_NO_THROW(sigproc << d1);
    sync();
    boost::filesystem::directory_iterator it_end;
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    auto count = 0;
    ASSERT_FALSE(it == boost::filesystem::directory_iterator());
    while (it != it_end)
    {
        boost::filesystem::path file = *it;
        ASSERT_EQ(file.extension().string(), ".fil");
        it++;
        count++;
    }
    ASSERT_EQ(d1.size(),count);
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
