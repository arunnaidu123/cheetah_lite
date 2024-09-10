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
#include "cheetah/modules/spsift/test/SpSiftTest.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spsift {
namespace test {


SpSiftTest::SpSiftTest()
    : ::testing::Test()
{
}

SpSiftTest::~SpSiftTest()
{
}

void SpSiftTest::SetUp()
{
}

void SpSiftTest::TearDown()
{
}

TEST_F(SpSiftTest, test_sift_dm)
{
    typedef data::SpCcl<uint8_t>::BlocksType::value_type::element_type TimeFrequencyType;

    std::size_t idx;

    //Create new SpCcl<uint8_t> instance
    data::SpCcl<uint8_t> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (idx=0; idx<10; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
        dm += typename Config::Dm(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    }


    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_thresh(17.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_thresh(0.025 * boost::units::si::seconds);
    float sigma_threshold = 8.0;
    config.dm_threshold(dm_thresh);
    config.pulse_width_threshold(width_thresh);
    config.sigma_threshold(sigma_threshold);

    SpSift sifter(config);

    sifter(cand_list);

    ASSERT_EQ(cand_list.size(), 5U);

    for (std::uint8_t ii = 0 ; ii < cand_list.size(); ++ii)
    {
        ASSERT_LE(dm_thresh.value() , cand_list[ii].dm().value());
    }
}

TEST_F(SpSiftTest, test_sift_width)
{
    typedef data::SpCcl<uint8_t>::BlocksType::value_type::element_type TimeFrequencyType;

    std::size_t idx;

    //Create new SpCcl<uint8_t> instance
    data::SpCcl<uint8_t> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for(idx=0; idx<10; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
        width += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.005 * boost::units::si::seconds);
    }


    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_thresh(10.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_thresh(0.025 * boost::units::si::seconds);
    float sigma_threshold = 8.0;
    config.dm_threshold(dm_thresh);
    config.pulse_width_threshold(width_thresh);
    config.sigma_threshold(sigma_threshold);

    SpSift sifter(config);

    sifter(cand_list);

    ASSERT_EQ(cand_list.size(), 5U);

    for(std::uint8_t ii = 0; ii < cand_list.size(); ++ii)
    {
        ASSERT_GT(width_thresh, cand_list[ii].width());
    }

}

TEST_F(SpSiftTest, test_sift_sigma)
{
    typedef data::SpCcl<uint8_t>::BlocksType::value_type::element_type TimeFrequencyType;

    std::size_t idx;

    //Create new SpCcl<uint8_t> instance
    data::SpCcl<uint8_t> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for(idx=0; idx<10; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
        sigma += 1.0;
    }


    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_thresh(10.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_thresh(0.025 * boost::units::si::seconds);
    float sigma_threshold = 15.0;
    config.dm_threshold(dm_thresh);
    config.pulse_width_threshold(width_thresh);
    config.sigma_threshold(sigma_threshold);

    SpSift sifter(config);

    sifter(cand_list);

    ASSERT_EQ(cand_list.size(), 5U);

    for (std::uint8_t ii = 0; ii < cand_list.size(); ++ii)
    {
        ASSERT_LE(sigma_threshold, (float) cand_list[ii].sigma());
    }

}

TEST_F(SpSiftTest, test_sift_many_candidates)
{
    typedef data::SpCcl<uint8_t>::BlocksType::value_type::element_type TimeFrequencyType;

    //Create new SpCcl<uint8_t> instance
    data::SpCcl<uint8_t> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 16.0;

    for(std::size_t idx=0; idx<100000; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
        sigma += 1.0;
    }


    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_thresh(10.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_thresh(0.025 * boost::units::si::seconds);
    float sigma_threshold = 15.0;
    config.dm_threshold(dm_thresh);
    config.pulse_width_threshold(width_thresh);
    config.sigma_threshold(sigma_threshold);
    config.maximum_candidates(50000);

    SpSift sifter(config);

    sifter(cand_list);

    ASSERT_EQ(cand_list.size(), 50000U);
}

TEST_F(SpSiftTest, test_sift_many_candidates_0threshold)
{
    typedef data::SpCcl<uint8_t>::BlocksType::value_type::element_type TimeFrequencyType;

    //Create new SpCcl<uint8_t> instance
    data::SpCcl<uint8_t> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for(std::size_t idx=0; idx<100000; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
        sigma += 1.0;
    }


    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_thresh(10.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_thresh(0.025 * boost::units::si::seconds);
    float sigma_threshold = 0.0;
    config.dm_threshold(dm_thresh);
    config.pulse_width_threshold(width_thresh);
    config.sigma_threshold(sigma_threshold);
    config.maximum_candidates(0);

    SpSift sifter(config);

    sifter(cand_list);

    ASSERT_EQ(cand_list.size(), 100000U);
}

} // namespace test
} // namespace spsift
} // namespace modules
} // namespace cheetah
} // namespace ska
