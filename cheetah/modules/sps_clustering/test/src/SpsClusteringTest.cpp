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
#include "cheetah/modules/sps_clustering/test/SpsClusteringTest.h"
#include "cheetah/modules/sps_clustering/SpsClustering.h"
#include "cheetah/modules/sps_clustering/Fof.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {
namespace test {


SpsClusteringTest::SpsClusteringTest()
    : ::testing::Test()
{
}

SpsClusteringTest::~SpsClusteringTest()
{
}

void SpsClusteringTest::SetUp()
{
}

void SpsClusteringTest::TearDown()
{
}

TEST_F(SpsClusteringTest, test_sps_clustering_no_tf_blocks)
{
    /* The test should verify that no clustering happens when no TF blocks are available */
    //Create new SpCcl<uint8_t> instance
    std::shared_ptr<data::SpCcl<uint8_t>> cand_list = std::make_shared<data::SpCcl<uint8_t>>();

    ASSERT_EQ(cand_list->tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list->push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.05*boost::units::si::seconds);
        dm += typename Config::Dm(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    }

    PANDA_LOG << "Reached Here...";
    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(2.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.01 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.01 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(1.732);
    // Run clustering
    SpsClustering merger(config);
    ASSERT_NO_THROW(merger(cand_list));

    // Check if it has done the right things
    ASSERT_EQ(cand_list->size(),10U);
}

TEST_F(SpsClusteringTest, test_sps_clustering_clustering_one_candidate)
{
    /* The test should verify that no clustering happens when no TF blocks are available */

    data::TimeFrequency<Cpu, uint8_t> tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));

    tf1.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));

    data::SpCcl<uint8_t>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);

    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf1));

    std::shared_ptr<data::SpCcl<uint8_t>> cand_list = std::make_shared<data::SpCcl<uint8_t>>(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<5; ++idx)
    {
        data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list->push_back(candidate);
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
        dm += 0.5 * pss::astrotypes::units::parsecs_per_cube_cm;
        sigma += 1.0;
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(5.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.01 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(1.0 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(1);
    // Run clustering
    SpsClustering merger(config);
    auto grouped_cands = merger(cand_list);

    // Check if it has done the right things
    ASSERT_EQ(grouped_cands->size(), 1U);
}

TEST_F(SpsClusteringTest, test_sps_clustering_multiple_candidates_within_limits)
{

    data::SpCcl<uint8_t>::BlocksType tf_v;
    data::TimeFrequency<Cpu, uint8_t> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf1));


    // Generate SpCcl instance
    std::shared_ptr<data::SpCcl<uint8_t>> cand_list = std::make_shared<data::SpCcl<uint8_t>>(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(5000.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.256 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {
        for (std::size_t ind=0; ind < 50; ++ind)
        {
            data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
            cand_list->push_back(candidate);
            tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.002*boost::units::si::seconds);
            width += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.002*boost::units::si::seconds);
            dm +=  10.0 * pss::astrotypes::units::parsecs_per_cube_cm;
            sigma += 1.0;
        }
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.1*boost::units::si::seconds);

    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(20.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.002 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.1 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(1.5);

    SpsClustering merger(config);
    auto grouped_cands = merger(cand_list);

    // Check if it has done the right things
    ASSERT_EQ(grouped_cands->size(),1U);
}

TEST_F(SpsClusteringTest, test_sps_clustering_multiple_candidates_beyond_limits)
{

    data::SpCcl<uint8_t>::BlocksType tf_v;
    data::TimeFrequency<Cpu, uint8_t> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, uint8_t>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(tf1));


    // Generate SpCcl instance
    std::shared_ptr<data::SpCcl<uint8_t>> cand_list = std::make_shared<data::SpCcl<uint8_t>>(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(5000.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.256 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {
        for (std::size_t ind=0; ind < 50; ++ind)
        {
            data::SpCcl<uint8_t>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
            cand_list->push_back(candidate);
            tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.002*boost::units::si::seconds);
            width += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.002*boost::units::si::seconds);
            dm +=  10.0 * pss::astrotypes::units::parsecs_per_cube_cm;
            sigma += 1.0;
        }
        tstart += data::SpCcl<uint8_t>::SpCandidateType::MsecTimeType(0.5*boost::units::si::seconds);

    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(20.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.002 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.1 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(1.5);

    SpsClustering merger(config);
    auto grouped_cands = merger(cand_list);

    // Check if it has done the right things
    ASSERT_EQ(grouped_cands->size(),10U);
}

} // namespace test
} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
