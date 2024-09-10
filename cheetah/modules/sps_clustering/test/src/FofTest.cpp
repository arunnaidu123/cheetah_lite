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
#include "cheetah/modules/sps_clustering/test/FofTest.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {
namespace test {


FofTest::FofTest()
    : ::testing::Test()
{
}

FofTest::~FofTest()
{
}

void FofTest::SetUp()
{
}

void FofTest::TearDown()
{
}

TEST_F(FofTest, test_constructor)
{
    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(17.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.025 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(10 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    Fof another_clusterer(config);
}

TEST_F(FofTest, test_empty_list)
{
    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(17.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.025 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(10 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);

    Fof clusterer(config);
    clusterer.linking_length(0.414);

    data::SpCcl<NumericalRep> cand_list;
    ASSERT_NO_THROW(clusterer(cand_list));
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),0U);
}




TEST_F(FofTest, test_group_one_candidate)
{

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));

    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.005 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<1; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
        dm += typename Config::Dm(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.01 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.001 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(10);
    config.num_threads(1);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),1U);
    ASSERT_EQ(groups[0].size(),1U);
}

TEST_F(FofTest, test_group_two_candidates)
{

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));


    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(1000.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.256 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<2; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.002 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.01 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(5);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),1U);
    ASSERT_EQ(groups[0].size(),2U);

}

TEST_F(FofTest, test_dm_grouping)
{

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));


    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    data::SpCcl<NumericalRep>::SpCandidateType::Dm dm(200.234 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.00734 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.00001*boost::units::si::seconds);
        dm += typename Config::Dm(0.0001 * pss::astrotypes::units::parsecs_per_cube_cm);
    }


    data::SpCcl<NumericalRep>::SpCandidateType::Dm dm2(263.245 * pss::astrotypes::units::parsecs_per_cube_cm);
    for (std::size_t idx=0; idx<10; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm2, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.00001*boost::units::si::seconds);
        dm2 += typename Config::Dm(0.0001 * pss::astrotypes::units::parsecs_per_cube_cm);
    }



    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.002 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.01 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(1.717);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),2U);
}

TEST_F(FofTest, test_group_multiple_candidates)
{

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));


    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(1000.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.256 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<500; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.002 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.01 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(5);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),1U);
    ASSERT_EQ(groups[0].size(),500U);

}

TEST_F(FofTest, test_width_negative_index)
{

    /* Tests that negative width index does not have any issues */
    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));

    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.002 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<5; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.01 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.001 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(10);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),1U);
    ASSERT_EQ(groups[0].size(),5U);
}

TEST_F(FofTest, test_dm_negative_index)
{

    /* Test that negative dm index will not have any issues */

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));

    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(0.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<5; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.01 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.001 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(10);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),1U);
    ASSERT_EQ(groups[0].size(),5U);
}

TEST_F(FofTest, test_time_negative_index)
{

    /* Test that negative time index will not have any problems */

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));

    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(12.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(0.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<5; ++idx)
    {
        data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.001*boost::units::si::seconds);
    }

    // Generate SpSift Config and construct SpSift object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.01 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.001 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(10);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),1U);
    ASSERT_EQ(groups[0].size(),5U);
}

TEST_F(FofTest, test_group_multiple_candidates_all_parameters)
{

    data::SpCcl<NumericalRep>::BlocksType tf_v;
    data::TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(data::TimeFrequency<Cpu, NumericalRep>::TimeType(1 * boost::units::si::milli * boost::units::si::seconds));
    tf_v.push_back(std::make_shared<data::TimeFrequency<Cpu, NumericalRep>>(tf1));


    // Generate SpCcl instance
    data::SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    //set single pulse candidate dispersion measure
    typename Config::Dm dm(50.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.0 ms
    typename Config::MsecTimeType width(0.256 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 10.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {
        for (std::size_t ind=0; ind < 50; ++ind)
        {
            data::SpCcl<NumericalRep>::SpCandidateType candidate(dm, tstart, width, sigma, idx);
            cand_list.push_back(candidate);
            tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.002*boost::units::si::seconds);
            width += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.002*boost::units::si::seconds);
            dm +=  1.0 * pss::astrotypes::units::parsecs_per_cube_cm;
        }
        tstart += data::SpCcl<NumericalRep>::SpCandidateType::MsecTimeType(0.1*boost::units::si::seconds);
    }

    // Generate SpsClustering Config and construct SpsClustering object
    Config config;
    typename Config::Dm dm_tolerance(1.0 * pss::astrotypes::units::parsecs_per_cube_cm);
    typename Config::MsecTimeType width_tolerance(0.002 * boost::units::si::seconds);
    typename Config::MsecTimeType time_tolerance(0.01 * boost::units::si::seconds);
    config.dm_tolerance(dm_tolerance);
    config.pulse_width_tolerance(width_tolerance);
    config.time_tolerance(time_tolerance);
    config.linking_length(1.5);
    Fof clusterer(config);
    auto groups = clusterer(cand_list);
    ASSERT_EQ(groups.size(),10U);
    ASSERT_EQ(groups[0].size(),50U);

}

} // namespace test
} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
