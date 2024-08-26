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
#include "cheetah/data/test/SpCclTest.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/SpCandidate.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/TimeFrequency.h"
#include "panda/Architecture.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace data {
namespace test {


SpCclTest::SpCclTest() : ::testing::Test()
{
}

SpCclTest::~SpCclTest()
{
}

void SpCclTest::SetUp()
{
}

void SpCclTest::TearDown()
{
}

// generate a sequence of contiguous in time TimeFrequency data blocks suitable for the SpCcl constructor
template<typename TimeFrequencyType>
std::vector<std::shared_ptr<TimeFrequencyType>> create_data_blocks(std::size_t number_of_blocks
                                                          , data::DimensionSize<data::Frequency> channels
                                                          , data::DimensionSize<data::Time> number_of_spectra
                                                          , typename TimeFrequencyType::TimeType sample_interval
                                                          , typename TimeFrequencyType::TimePointType start_time = typename TimeFrequencyType::TimePointType(pss::astrotypes::units::julian_day(100.0))
                                                          )
{
    typename TimeFrequencyType::TimeType block_interval(number_of_spectra * sample_interval);

    std::vector<std::shared_ptr<TimeFrequencyType>> rv;
    for(std::size_t count=0; count < number_of_blocks; ++count)
    {
        // create a single block and set its meta data
        typename std::remove_const<TimeFrequencyType>::type tf(number_of_spectra, channels);
        std::fill(tf.begin(), tf.end(), count);
        tf.sample_interval(sample_interval);
        tf.start_time(start_time);
        start_time += block_interval;

        rv.emplace_back(std::make_shared<TimeFrequencyType>(tf));
    }
    return rv;
}

/**
 * Test construction of SpCcl<uint8_t> object
 */
TEST_F(SpCclTest, test_tf_blocks)
{
    typedef uint8_t NumericalRep;
    typedef typename SpCcl<NumericalRep>::TimeFrequencyType TimeFrequencyType;

    //Generate two blocks of TF data and push to the block list
    typename SpCcl<NumericalRep>::BlocksType blocks;
    {
        std::shared_ptr<TimeFrequencyType> b1(new TimeFrequencyType(data::DimensionSize<data::Time>(1000), data::DimensionSize<data::Frequency>(10)));
        std::shared_ptr<TimeFrequencyType> b2(new TimeFrequencyType(data::DimensionSize<data::Time>(100), data::DimensionSize<data::Frequency>(10)));
        blocks.emplace_back(b1);
        blocks.emplace_back(b2);
    }

    //Create new SpCcl<NumericalRep> instance
    SpCcl<NumericalRep> cand_list(blocks, data::DimensionIndex<data::Time>(0));

    //Retreive tf_blocks
    auto const& blocks_ref = cand_list.tf_blocks();

    ASSERT_EQ(blocks_ref.size(), std::size_t(2));
    ASSERT_EQ(blocks_ref[0]->number_of_spectra(), std::size_t(1000));
    ASSERT_EQ(blocks_ref[1]->number_of_spectra(), std::size_t(100));
}

TEST_F(SpCclTest, test_candidate_vector_behaviour)
{
    typedef uint8_t NumericalRep;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typedef SpCcl<NumericalRep>::BlocksType::value_type::element_type TimeFrequencyType;

    std::size_t idx;

    //Create new SpCcl<NumericalRep> instance
    SpCcl<NumericalRep> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;

    for (idx=0; idx<10; ++idx)
    {
        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    for (idx=0; idx<10; ++idx)
    {
        auto const& cand = cand_list[idx];
        ASSERT_EQ(cand.dm(), dm);
        ASSERT_EQ(cand.tstart(), tstart);
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand.ident(), idx);
    }

    SpCcl<NumericalRep> cand_list2;

    for (idx=0; idx<10; ++idx)
    {
        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list2.emplace_back(std::move(candidate));
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    for (idx=0; idx<10; ++idx)
    {
        auto const& cand = cand_list2[idx];
        ASSERT_EQ(cand.dm(), dm);
        ASSERT_EQ(cand.tstart(), tstart);
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand.ident(), idx);
    }
}

TEST_F(SpCclTest, test_candidate_dm_range)
{
    typedef uint8_t NumericalRep;
    typedef SpCandidate<Cpu, float> SpCandidateType;

    std::size_t idx;

    //Create new SpCcl<NumericalRep> instance
    SpCcl<NumericalRep> cand_list;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;

    for (idx=0; idx<10; ++idx)
    {
        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        dm += SpCandidateType::Dm(1.0 * parsecs_per_cube_cm);
        cand_list.push_back(candidate);
    }

    ASSERT_EQ(cand_list.dm_range().first, SpCandidateType::Dm(12.0 * parsecs_per_cube_cm));
    ASSERT_EQ(cand_list.dm_range().second, SpCandidateType::Dm(21.0 * parsecs_per_cube_cm));

    // Test removing candidates with the lowest Dm
    cand_list.remove_if([](SpCandidateType const& s)
                        {
                            return s.dm() == SpCandidateType::Dm(12.0 * parsecs_per_cube_cm);
                        });
    ASSERT_EQ(cand_list.dm_range().first, SpCandidateType::Dm(13.0 * parsecs_per_cube_cm));
    ASSERT_EQ(cand_list.dm_range().second, SpCandidateType::Dm(21.0 * parsecs_per_cube_cm));

    // Test removing candidates with the highest Dm
    cand_list.remove_if([](SpCandidateType const& s)
                        {
                            return s.dm() == SpCandidateType::Dm(21.0 * parsecs_per_cube_cm);
                        });
    ASSERT_EQ(cand_list.dm_range().first, SpCandidateType::Dm(13.0 * parsecs_per_cube_cm));
    ASSERT_EQ(cand_list.dm_range().second, SpCandidateType::Dm(20.0 * parsecs_per_cube_cm));

    // Test removing candidates neither highest or lowest
    cand_list.remove_if([](SpCandidateType const& s)
                        {
                            return s.dm() == SpCandidateType::Dm(17.0 * parsecs_per_cube_cm);
                        });
    ASSERT_EQ(cand_list.dm_range().first, SpCandidateType::Dm(13.0 * parsecs_per_cube_cm));
    ASSERT_EQ(cand_list.dm_range().second, SpCandidateType::Dm(20.0 * parsecs_per_cube_cm));

}

TEST_F(SpCclTest, test_equalto_operator_no_data)
{
/**
 *@brief  Test for when there are no candidate, the object should return just iterator equal to the end iterator and not do any extraction
 */
    // Generate SpCcl instance
    typedef uint8_t NumericalRep;
    SpCcl<NumericalRep> cand_list;
    CandidateWindow window;
    ASSERT_NO_THROW(cand_list.data_begin(window));
    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();
    SpCcl<NumericalRep>::ConstDataIterator it_end2 = cand_list.data_end();

    ASSERT_TRUE(it_end == it_end2);
    ASSERT_FALSE(it_end != it_end2);
    ASSERT_TRUE(it_begin == it_end);
}

TEST_F(SpCclTest, test_emplace_back)
{
    typedef uint8_t NumericalRep;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    float sigma = 20.0;

    // just add a single candidate
    SpCcl<NumericalRep> cand_list;
    SpCandidateType candidate(dm, tstart, width, sigma);
    cand_list.emplace_back(std::move(candidate));
    ASSERT_EQ(cand_list.size(), 1U);
    auto const& cand = cand_list[0];
    ASSERT_EQ(cand.dm(), dm);
    ASSERT_EQ(cand.tstart(), tstart);
    ASSERT_EQ(cand.width(), width);
    ASSERT_EQ(cand.sigma(), sigma);

    //  add an out of order candidate
    SpCandidateType::MsecTimeType tstart2(1.0 * boost::units::si::seconds);
    SpCandidateType candidate2(dm, tstart2, width, sigma);
    cand_list.emplace_back(std::move(candidate2));
    ASSERT_EQ(cand_list.size(), 2U);
    auto const& cand0 = cand_list[0];
    auto const& cand1 = cand_list[1];
    ASSERT_EQ(cand0.tstart(), tstart2);
    ASSERT_EQ(cand1.tstart(), tstart);
}

TEST_F(SpCclTest, test_emplace_back_constructor_list)
{
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    float sigma = 20.0;

    data::DimensionSize<data::Time> number_of_spectra(4U);
    data::DimensionSize<data::Frequency> number_of_channels(10U);

    auto f1 =  TimeFrequencyType::FrequencyType(10.0 * boost::units::si::hertz);
    auto f2 =  TimeFrequencyType::FrequencyType(20.0 * boost::units::si::hertz);
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);
    boost::units::quantity<MilliSeconds, double> delay =
        (4.15 * 1000000.0 * boost::units::si::milli * boost::units::si::seconds) * ( dm * ( (1/(f1 * f1)) - (1/(f2 * f2)) )).value();

    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_spectra, number_of_channels);
    tf->set_channel_frequencies_const_width( f2, -1.0 * delta );

    SpCcl<NumericalRep> cand_list({tf}, data::DimensionIndex<data::Time>(0));
    cand_list.emplace_back(dm, tstart, width, sigma);

    ASSERT_EQ(cand_list.size(), 1U);
    auto const& cand = cand_list[0];
    ASSERT_EQ(cand.dm(), dm);
    ASSERT_EQ(cand.tstart(), tstart);
    ASSERT_DOUBLE_EQ(cand.tend().value(), (tstart + delay).value());
    ASSERT_EQ(cand.width(), width);
    ASSERT_EQ(cand.sigma(), sigma);
}

TEST_F(SpCclTest, test_sort_behaviour)
{
    /**
    *@brief  Check if the candidates are sorted according to start time when inserted with emplace()
    */
    typedef uint8_t NumericalRep;
    typedef SpCandidate<Cpu, float> SpCandidateType;

    std::size_t idx;

    //Create new SpCcl<NumericalRep> instance
    SpCcl<NumericalRep> cand_list;

    SpCcl<NumericalRep> cand_list2;

    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(0));

    //set single pulse candidate dispersion measure
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.0 seconds
    SpCandidateType::MsecTimeType tstart(2.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;

    for (idx=0; idx<10; ++idx)
    {

        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back(candidate);

        cand_list2.emplace(std::move(candidate));

        tstart += SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    SpCandidateType::MsecTimeType ststart(2.0 * boost::units::si::seconds);
    for (idx=0; idx<10; ++idx)
    {
        SCOPED_TRACE(idx);
        auto const& cand = cand_list[idx];
        auto const& cand2 = cand_list2[idx];
        ASSERT_EQ(cand.dm(), dm);
        ASSERT_EQ(cand2.dm(), dm);
        ASSERT_EQ(cand.tstart(), ststart);
        ASSERT_EQ(cand2.tstart(), ststart);
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand2.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand2.sigma(), sigma);
        ASSERT_EQ(cand.ident(), idx);
        ASSERT_EQ(cand2.ident(), idx);
        ststart += SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
    }
}

TEST_F(SpCclTest, test_disordered_input_sort_behaviour)
{

    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;

    data::DimensionSize<data::Time> number_of_spectra(4U);
    data::DimensionSize<data::Frequency> number_of_channels(10U);

    auto f1 =  TimeFrequencyType::FrequencyType(10.0 * boost::units::si::hertz);
    auto f2 =  TimeFrequencyType::FrequencyType(20.0 * boost::units::si::hertz);
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);


    std::shared_ptr<TimeFrequencyType> tf = std::make_shared<TimeFrequencyType>(number_of_spectra, number_of_channels);
    tf->set_channel_frequencies_const_width( f2, -1.0 * delta );
    ASSERT_EQ(tf->low_high_frequencies().first, f1);
    ASSERT_EQ(tf->low_high_frequencies().second, f2);

    //Create new SpCcl<NumericalRep> instance
    SpCcl<NumericalRep> cand_list({tf}, data::DimensionIndex<data::Time>(0));
    ASSERT_EQ(cand_list.tf_blocks().size(),std::size_t(1));
    ASSERT_EQ(cand_list.tf_blocks()[0]->low_high_frequencies().first, f1);

    SpCcl<NumericalRep> cand_list2({tf}, data::DimensionIndex<data::Time>(0));
    ASSERT_EQ(cand_list2.tf_blocks().size(),std::size_t(1));

    //set single pulse candidate dispersion measure
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    boost::units::quantity<MilliSeconds, double> delay =
        (4.15 * 1000000.0 * boost::units::si::milli * boost::units::si::seconds) * ( dm * ( (1/(f1 * f1)) - (1/(f2 * f2)) )).value();

    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(11.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {

        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        cand_list.push_back_calculate_duration(candidate);
        cand_list2.emplace_calculate_duration(std::move(candidate));
        tstart -= SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    SpCandidateType::MsecTimeType ststart(2.0 * boost::units::si::seconds);
    for (std::size_t idx=0; idx<10; ++idx)
    {
        SCOPED_TRACE(idx);
        auto const& cand = cand_list[idx];
        auto const& cand2 = cand_list2[idx];
        ASSERT_EQ(cand.dm(), dm);
        ASSERT_EQ(cand2.dm(), dm);
        ASSERT_EQ(cand.tstart(), ststart);
        ASSERT_DOUBLE_EQ(cand.tend().value(), (ststart + delay).value());
        ASSERT_EQ(cand2.tstart(), ststart);
        ASSERT_DOUBLE_EQ(cand2.tend().value(), (ststart + delay).value());
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand2.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand2.sigma(), sigma);
        ASSERT_EQ(cand.ident(), 9-idx);
        ASSERT_EQ(cand2.ident(), 9-idx);
        ststart += SpCandidateType::MsecTimeType(1.0*boost::units::si::seconds);
    }
}


TEST_F(SpCclTest, test_candidates_extraction)
{
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;

    TimeFrequencyType tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));
    TimeFrequencyType tf2(data::DimensionSize<data::Time>(200),data::DimensionSize<data::Frequency>(1024));
    TimeFrequencyType tf3(data::DimensionSize<data::Time>(300),data::DimensionSize<data::Frequency>(1024));

    tf1.sample_interval(TimeFrequencyType::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf2.sample_interval(TimeFrequencyType::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));
    tf3.sample_interval(TimeFrequencyType::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));

    SpCcl<NumericalRep>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);
    std::fill(tf2.begin(),tf2.end(),1);
    std::fill(tf3.begin(),tf3.end(),2);

    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));
    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf2));
    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf3));

    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(1 * boost::units::si::milli * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(5.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;
    //set the tend;
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);

    for (std::uint32_t idx=0; idx<30; ++idx)
    {
        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        candidate.duration_end(tend);
        cand_list.push_back(candidate);
        tstart += SpCandidateType::MsecTimeType(10*boost::units::si::seconds);
    }

    // Run Data iterator instance here to extract candidates

    CandidateWindow window( 1000 * data::milliseconds, 1000 * data::milliseconds);



    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin( window );
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    unsigned count=0;
    while(it_begin != it_end)
    {
        // Check output
        auto slice = *it_begin;
        auto start_time = it_begin.start_time();
        ASSERT_EQ(static_cast<std::size_t>(slice.size<data::Frequency>()),tf1.number_of_channels());
        ASSERT_EQ(static_cast<std::size_t>(slice.size<data::Time>()), 4); // 1ms before and after for windowing, 2 ms wide candidates = 4ms
        double ts = (pss::astrotypes::units::duration_cast<boost::units::quantity<MilliSeconds, double>>(start_time - tf1.start_time())).value();
        double cts = (cand_list[count].tstart() - window.ms_before()).value();
        ASSERT_DOUBLE_EQ(ts, cts);
        ASSERT_NO_THROW(++it_begin);
        ++count;
    }

    ASSERT_EQ(count,cand_list.size());
}

TEST_F(SpCclTest, test_candidate_extraction_begin_range)
{
    /*
     * @brief Test case where the candidate is either at the beginning of the block
     *         or the end of the block. The test should resize the candidate window
     *         before extracting the slice of data.
     */
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    TimeFrequency<Cpu, NumericalRep> tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));

    tf1.sample_interval(TimeFrequencyType::TimeType(1*boost::units::si::seconds));
    float sigma=20.0;

    SpCcl<NumericalRep>::BlocksType tf_v;
    // fill data
    unsigned n=0;
    std::generate(tf1.begin(),tf1.end(),[&](){return ++n;});

    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));
    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to the beginning
    SpCandidateType::MsecTimeType tstart3(0.1 * boost::units::si::seconds);
    // set the end time
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);



    SpCandidateType candidate3(dm,tstart3,width,sigma, 3);
    candidate3.duration_end(tend);
    cand_list.push_back(candidate3);

    CandidateWindow window(150 * data::milliseconds,150 * data::milliseconds);

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    //unsigned ind=0;
    auto tf_it  = tf1.begin();
    auto sl_it = (*it_begin).begin();


    for (uint32_t ii=0; ii < 5120 ; ++ii )
    {
        ASSERT_EQ(*sl_it,*tf_it);
        ++tf_it;
        ++sl_it;
    }

    unsigned count=0;
    while(it_begin != it_end)
    {
        ASSERT_NO_THROW(++it_begin);
        ++count;
    }

    ASSERT_EQ(count,cand_list.size());
}

TEST_F(SpCclTest, test_candidate_extraction_end_range)
{
    /*
     * @brief: Test case where the candidate is either at the beginning of the block
     *         or the end of the block. The test should resize the candidate window
     *         before extracting the slice of data.
     */
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typedef SpCcl<NumericalRep>::BlocksType::value_type::element_type TimeFreqType;
    TimeFrequencyType tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));

    tf1.sample_interval(TimeFrequencyType::TimeType(1*boost::units::si::seconds));
    float sigma=20.0;

    SpCcl<NumericalRep>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);

    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));
    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to the end
    SpCandidateType::MsecTimeType tstart(99.9 * boost::units::si::seconds);
    // set end time
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);

    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    candidate.duration_end(tend);
    cand_list.push_back(candidate);

    CandidateWindow window(2000 * data::milliseconds,2000 * data::milliseconds);

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    auto tf_it  = tf1.end();
    tf_it = tf_it - 2048;
    auto sl_it = (*it_begin).begin();

    for (uint32_t ii=0; ii < 2048; ++ii )
    {
        ASSERT_EQ(*sl_it,*tf_it);
        ++tf_it;
        ++sl_it;
    }
    unsigned count=0;
    while(it_begin != it_end)
    {
        ASSERT_NO_THROW(++it_begin);
        ++count;
    }

    ASSERT_EQ(count,cand_list.size());
}

TEST_F(SpCclTest, test_candidate_extraction_overlap)
{
    /*
     * @brief: Test when there are two candidates within the window of the first candidate.
     *         The expected behaviour is that the extractor should ignore the candidate i.e should not repeat the same data
     *         and move on to the next one.
     */
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    TimeFrequencyType tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));

    tf1.sample_interval(TimeFrequencyType::TimeType(1*boost::units::si::seconds));
    float sigma=20.0;

    SpCcl<NumericalRep>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);

    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));
    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(2));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to the beginning, middle and the end
    SpCandidateType::MsecTimeType tstart(30 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart2(29 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart3(31 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart4(36 * boost::units::si::seconds);
    // set tend
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);


    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    SpCandidateType candidate2(dm,tstart2,width,sigma, 2);
    SpCandidateType candidate3(dm,tstart3,width,sigma, 3);
    SpCandidateType candidate4(dm,tstart4,width,sigma, 4);
    candidate.duration_end(tend);
    candidate2.duration_end(tend);
    candidate3.duration_end(tend);
    candidate4.duration_end(tend);
    cand_list.push_back(candidate);
    cand_list.push_back(candidate2);
    cand_list.push_back(candidate3);
    cand_list.push_back(candidate4);

    CandidateWindow window(1000 * data::milliseconds,1000 * data::milliseconds);

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    unsigned count=0;
    while(it_begin != it_end)
    {
        ASSERT_NO_THROW(++it_begin);
        count++;
    }
    ASSERT_EQ(count,2);
}


TEST_F(SpCclTest, test_candidate_extraction_before_start_of_the_block)
{
    /*
     * @brief: Test when there are two candidates within the window of the first candidate.
     *         The expected behaviour is that the extractor should ignore the candidate i.e should not repeat the same data
     *         and if the start time is earlier than the start time of the block, the start time should get update.
     */
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    TimeFrequencyType tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    tf1.sample_interval(TimeFrequencyType::TimeType(1*boost::units::si::seconds));
    tf1.start_time(epoch);
    float sigma=20.0;

    SpCcl<NumericalRep>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);

    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));
    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(1));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    SpCandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the single pulse start time to the beginning, middle and the end
    SpCandidateType::MsecTimeType tstart(1 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart2(3 * boost::units::si::seconds);
    SpCandidateType::MsecTimeType tstart3(2 * boost::units::si::seconds);
    // set tend
    SpCandidateType::MsecTimeType tend(1000 * boost::units::si::milli * boost::units::si::seconds);


    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    SpCandidateType candidate2(dm,tstart2,width,sigma, 2);
    SpCandidateType candidate3(dm,tstart3,width,sigma, 3);
    candidate.duration_end(tend);
    candidate2.duration_end(tend);
    candidate3.duration_end(tend);
    cand_list.push_back(candidate);
    cand_list.push_back(candidate2);
    cand_list.push_back(candidate3);

    CandidateWindow window(5000 * data::milliseconds,5000 * data::milliseconds);

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    while(it_begin != it_end)
    {
        auto start_time = it_begin.start_time();
        ASSERT_EQ(start_time, tf1.start_time());

        ASSERT_NO_THROW(++it_begin);
    }
    ASSERT_EQ(1U,cand_list.size()-2);

}

TEST_F(SpCclTest, test_candidate_spanning_multiple_blocks)
{
    /*
     * @brief: Tests whether a candidate that spans multiple blocks is extracted properly.
     *         checks that the data extracted match with the candidate
     */
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typedef SpCandidateType::MsecTimeType MilliSeconds;

    // set the parmeters in the test
    const data::DimensionSize<data::Frequency> number_of_channels(1024);
    const data::DimensionSize<data::Time> number_of_spectra(100);
    const data::DimensionIndex<data::Time> offset(0);
    MilliSeconds sample_interval(1000 * data::milliseconds);
    const std::size_t number_of_blocks = 3;

    // set up the structure with the parameters to test
    typename SpCcl<NumericalRep>::BlocksType tf_v =
            create_data_blocks<const TimeFrequencyType>(number_of_blocks
                                      , number_of_channels
                                      , number_of_spectra
                                      , TimeFrequencyType::TimeType(sample_interval)
                                      , TimeFrequencyType::TimePointType(pss::astrotypes::units::julian_day(0))
                                      );

    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, offset);

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(1 * data::milliseconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(5 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;
    //set end time
    SpCandidateType::MsecTimeType tend(2000 * data::milliseconds);



    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    candidate.duration_end(tend);
    cand_list.push_back(candidate);

    CandidateWindow window(2000 * data::milliseconds,200000 * data::milliseconds);

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    // Check if we get all the data where the candidate exists
    auto tf_it  = tf_v[0]->begin();
    tf_it = tf_it + 48;
    auto sl_it1 = (*it_begin).begin();
    for (uint32_t ii=0; ii < 1552; ++ii)
    {
        ASSERT_EQ(*sl_it1,*tf_it);
        ++sl_it1;
        ++tf_it;
    }
    ++it_begin;
    auto sl_it2 = (*it_begin).begin();
    auto tf_it2 = tf_v[1]->begin();
    for (uint32_t ii=0; ii < 1600; ++ii)
    {
        ASSERT_EQ(*sl_it2,*tf_it2);
        ++sl_it2;
        ++tf_it2;
    }
    ++it_begin;
    auto sl_it3 = (*it_begin).begin();
    auto tf_it3 = tf_v[2]->begin();
    for (uint32_t ii=0 ; ii < 112; ++ii)
    {
        ASSERT_EQ(*sl_it3,*tf_it3);
        ++sl_it3;
        ++tf_it3;
    }

    ASSERT_NO_THROW(++it_begin);

}

TEST_F(SpCclTest, test_candidate_start_time)
{
    // We test the start time of each candidate is in the correct relative location to the iterator blocks
    // start_time() method
    // This scenario tested is for well seperated blocks each with a single candidate
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typedef SpCandidateType::MsecTimeType MilliSeconds;

    // set the parmeters in the test
    const data::DimensionSize<data::Frequency> number_of_channels(1024);
    const data::DimensionSize<data::Time> number_of_spectra(100);
    const data::DimensionIndex<data::Time> offset(2);
    MilliSeconds sample_interval(1000 * data::milliseconds);
    const std::size_t number_of_blocks = 3;

    // set up the structure with the parameters to test
    typename SpCcl<NumericalRep>::BlocksType tf_v =
            create_data_blocks<const TimeFrequencyType>(number_of_blocks
                                      , number_of_channels
                                      , number_of_spectra
                                      , TimeFrequencyType::TimeType(sample_interval)
                                      , TimeFrequencyType::TimePointType(pss::astrotypes::units::julian_day(0))
                                      );

    // Generate SpCcl instance with offset of 10 seconds
    SpCcl<NumericalRep> cand_list(tf_v, offset);

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(1 * data::milliseconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(5.0 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;
    //set the tend;
    SpCandidateType::MsecTimeType tend(2000 * data::milliseconds);

    for (std::uint32_t idx=0; idx<30; ++idx)
    {
        SpCandidateType candidate(dm, tstart, width, sigma, idx);
        candidate.duration_end(tend);
        cand_list.push_back(candidate);
        tstart += SpCandidateType::MsecTimeType(10*boost::units::si::seconds);
    }

    // Run Data iterator instance here to extract candidates

    CandidateWindow window( 1000 * data::milliseconds, 1000 * data::milliseconds);


    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin( window );
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    unsigned count=0;
    while(it_begin != it_end)
    {
        // Check output
        auto slice = *it_begin;
        auto start_time = it_begin.start_time();
        ASSERT_EQ(static_cast<std::size_t>(slice.size<data::Frequency>()), number_of_channels);
        ASSERT_EQ(static_cast<std::size_t>(slice.size<data::Time>()), 4); // 1s before and after for windowing, 2s wide candidates = 4s
        double ts = (pss::astrotypes::units::duration_cast<boost::units::quantity<data::MilliSeconds, double>>(start_time - tf_v[0]->start_time())).value();
        double cts = (cand_list[count].tstart() + cand_list.offset_time() - window.ms_before()).value();
        ASSERT_DOUBLE_EQ(ts, cts);
        ASSERT_NO_THROW(++it_begin);
        ++count;
    }

    ASSERT_EQ(count,cand_list.size());
}


TEST_F(SpCclTest, test_candidate_start_beyond_end)
{
    /*
     * @brief: Tests whether a candidate that starts beyond the end of block throws an exception.
     *
     */
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidate<Cpu, float> SpCandidateType;
    TimeFrequencyType tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));

    tf1.sample_interval(TimeFrequencyType::TimeType(1000 * boost::units::si::milli * boost::units::si::seconds));

    SpCcl<NumericalRep>::BlocksType tf_v;
    // fill data
    std::fill(tf1.begin(),tf1.end(),0);

    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));

    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    SpCandidateType::MsecTimeType width(1 * boost::units::si::milli * boost::units::si::seconds);
    //set the single pulse start time to 105 seconds
    SpCandidateType::MsecTimeType tstart(105 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;
    //set end time
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);



    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    candidate.duration_end(tend);
    cand_list.push_back(candidate);

    CandidateWindow window(2000 * data::milliseconds,2000 * data::milliseconds);

    // Check if exception is thrown
    ASSERT_ANY_THROW(cand_list.data_begin(window));

}

TEST_F(SpCclTest, test_no_candidates)
{
/**
 *@brief  Test for when there are no candidate, the object should return just iterator equal to the end iterator and not do any extraction
 */
    // Generate SpCcl instance
    typedef uint8_t NumericalRep;
    SpCcl<NumericalRep> cand_list;
    CandidateWindow window;
    ASSERT_NO_THROW(cand_list.data_begin(window));
    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();
    while (it_begin != it_end)
    {
        ASSERT_NO_THROW(++it_begin);
    }

}

TEST_F(SpCclTest, test_offset_time)
{
    typedef SpCcl<uint8_t> SpCclType;
    typedef SpCclType::BlocksType BlocksType;
    typedef BlocksType::value_type::element_type TimeFrequencyType;

    TimeFrequencyType::TimeType sample_interval(64 * boost::units::si::milli * boost::units::si::seconds);

    BlocksType tf_v;
    typename std::remove_const<TimeFrequencyType>::type tf1(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(16));
    tf1.sample_interval(sample_interval);
    tf_v.push_back(std::make_shared<TimeFrequencyType>(tf1));

    for(data::DimensionIndex<data::Time> offset(0); offset < tf1.number_of_spectra(); ++offset) {
        SpCclType cand_list(tf_v, offset);
        ASSERT_DOUBLE_EQ(pss::astrotypes::units::duration_cast<TimeFrequencyType::TimeType>(cand_list.offset_time()).value(), (offset * sample_interval).value());
    }
}

TEST_F(SpCclTest, test_candidate_start_multiple_blocks_behind)
{
    /*
     * @brief: Tests whether a candidate that has the window spanning multiple blocks behind the actual start time is extracted properly.
     *         checks that the data extracted match with the candidate
     */
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidateType::MsecTimeType MilliSeconds;

    // set the parmeters in the test
    const data::DimensionSize<data::Time> number_of_spectra(100);
    const data::DimensionSize<data::Frequency> number_of_channels(16);
    const data::DimensionIndex<data::Time> offset(5);
    const std::size_t number_of_blocks = 12;
    MilliSeconds sample_interval(1000 * data::milliseconds);

    // set up the structure with the parameters to test
    typename SpCcl<NumericalRep>::BlocksType tf_v =
            create_data_blocks<const TimeFrequencyType>(number_of_blocks
                                                      , number_of_channels
                                                      , number_of_spectra
                                                      , TimeFrequencyType::TimeType(sample_interval));

    // Generate SpCcl instance
    SpCcl<NumericalRep> cand_list(tf_v, data::DimensionIndex<data::Time>(0));

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(1 * boost::units::si::milli * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(250 * boost::units::si::seconds);
    //set the candidate significance
    float sigma = 20.0;
    //set end time
    SpCandidateType::MsecTimeType tend(2000 * data::milliseconds);



    SpCandidateType candidate(dm, tstart, width, sigma, 1);
    candidate.duration_end(tend);
    cand_list.push_back(candidate);

    CandidateWindow window(200000 * data::milliseconds,10000 * data::milliseconds);

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    // Check if we get all the data where the candidate exists
    auto tf_it  = tf_v[0]->begin();
    tf_it = tf_it + 800;
    auto sl_it1 = (*it_begin).begin();
    for (uint32_t ii=0; ii < 800; ++ii)
    {
        ASSERT_EQ(*sl_it1,*tf_it);
        ++sl_it1;
        ++tf_it;
    }
    ++it_begin;
    auto sl_it2 = (*it_begin).begin();
    auto tf_it2 = tf_v[1]->begin();
    for (std::size_t ii=0; ii < 1600; ++ii)
    {
        ASSERT_EQ(*sl_it2,*tf_it2);
        ++sl_it2;
        ++tf_it2;
    }
    ++it_begin;
    auto sl_it3 = (*it_begin).begin();
    auto tf_it3 = tf_v[2]->begin();
    for (std::size_t ii=0 ; ii < 992; ++ii)
    {
        ASSERT_EQ(*sl_it3,*tf_it3);
        ++sl_it3;
        ++tf_it3;
    }

    ASSERT_NO_THROW(++it_begin);

}

/* A test that tests a full real-life scenario that includes candidates between blocks
 * spanning multiple blocks and overlapping candidates inside a Ccl with a non-zero offset
 */
TEST_F(SpCclTest, test_everything)
{
    typedef SpCandidate<Cpu, float> SpCandidateType;
    typedef uint8_t NumericalRep;
    typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef SpCandidateType::MsecTimeType MilliSeconds;

    // test with a large window
    CandidateWindow window(2000 * data::milliseconds, 2000 * data::milliseconds);

    // set the parmeters in the test
    const data::DimensionSize<data::Time> number_of_spectra(50);
    const data::DimensionSize<data::Frequency> number_of_channels(16);
    const data::DimensionIndex<data::Time> offset(5);
    const std::size_t number_of_blocks = 12;
    MilliSeconds sample_interval(1000 * data::milliseconds);
    MilliSeconds offset_time(offset * sample_interval);
    MilliSeconds block_interval(number_of_spectra * sample_interval);

    // set up the structure with the parameters to test
    typename SpCcl<NumericalRep>::BlocksType tf_v =
            create_data_blocks<const TimeFrequencyType>(number_of_blocks
                                                      , number_of_channels
                                                      , number_of_spectra
                                                      , TimeFrequencyType::TimeType(sample_interval));


    // Generate SpCcl instance with an offset of 5 seconds
    SpCcl<NumericalRep> cand_list(tf_v, offset);

    // Generate Candidates
    typename SpCandidateType::Dm dm(12.0 * parsecs_per_cube_cm);
    //set the single pulse candidate width to 1 ms
    MilliSeconds width(1 * data::milliseconds);
    //set the single pulse start time to 2.23 seconds
    MilliSeconds tstart(1000 * data::milliseconds);
    //set the candidate significance
    float sigma = 20.0;
    //set end time
    MilliSeconds tend(2000 * data::milliseconds);


    std::vector<MilliSeconds> expected_block_start { tstart - window.ms_before() }; // expect first block to start at offset position - window size
    std::vector<int> expected_candidates_in_block { 10 }; // first block should have 10 candidates
    std::vector<int> expected_spectra_in_block { 15 };

    // generate 10 candidate pulses with a period of 1000 milliseconds
    // with the given windowing size we expect all these candidates to be in the same block
    for (std::size_t ii=0; ii < 10; ++ii)
    {
        SpCandidateType candidate(dm, tstart, width, sigma, 1);
        candidate.duration_end(tend);
        cand_list.push_back(candidate);
        tstart += (1000 * data::milliseconds);
    }

    // Adding two overlapping candidates outside of the window of the first set of candidates and make them span multiple blocks
    const MilliSeconds tstart_new = 3 * block_interval + 4.0 * sample_interval - offset_time; // start 4 samples inside the 3rd block
    MilliSeconds tstart_new_copy = tstart_new;

    std::size_t blocks_to_span = 4;
    const MilliSeconds tend_new(blocks_to_span * block_interval);

    expected_block_start.push_back(tstart_new - window.ms_before());
    for (std::size_t ii=0; ii < 2; ++ii)
    {
        SpCandidateType candidate(dm, tstart_new_copy, width, sigma, 1);
        candidate.duration_end(tend_new);
        cand_list.push_back(candidate);
        tstart_new_copy += block_interval; // next pulse has a large offset
    }
    expected_candidates_in_block.push_back(2);
    expected_spectra_in_block.push_back(number_of_spectra - 4 + 2); // a full blocks worth - the start offset + window
    expected_block_start.push_back(expected_block_start.back() + expected_spectra_in_block.back() * sample_interval);
    for(std::size_t block=0; block < blocks_to_span ; ++block)
    {
        expected_block_start.push_back(expected_block_start.back() + block_interval);
        expected_candidates_in_block.push_back(expected_candidates_in_block.back());
        expected_spectra_in_block.push_back(number_of_spectra);
    }
    expected_candidates_in_block.push_back(expected_candidates_in_block.back());
    expected_spectra_in_block.push_back(4 + 2); // 4 from the start offset and 2 from the windowing

    // end with some overlapping candidates that span a block and go beyond the end of the data block
    MilliSeconds tstart_newest((tf_v.size() - 2) * block_interval + sample_interval - offset_time);
    blocks_to_span = 2;
    MilliSeconds tend_newest(blocks_to_span * block_interval);

    MilliSeconds block_end;
    expected_block_start.push_back(tstart_newest - window.ms_before());
    for (std::size_t ii=0; ii < 4; ++ii)
    {
        SpCandidateType candidate(dm, tstart_newest, width, sigma, 1);
        candidate.duration_end(tend_newest);
        cand_list.push_back(candidate);
        block_end = tstart_newest + tend_newest;
        tstart_newest += 1000 * data::milliseconds;
    }
    expected_candidates_in_block.push_back(4);
    expected_spectra_in_block.push_back(1); // with windowing we should be one spectra into the previous data block
    expected_block_start.push_back(expected_block_start.back() + expected_spectra_in_block.back() * sample_interval);
    for(std::size_t block=0; block < blocks_to_span; ++block)
    {
        expected_block_start.push_back(expected_block_start.back() + block_interval);
        expected_candidates_in_block.push_back(expected_candidates_in_block.back());
        expected_spectra_in_block.push_back(number_of_spectra);
    }
    expected_candidates_in_block.push_back(expected_candidates_in_block.back());

    SpCcl<NumericalRep>::ConstDataIterator it_begin = cand_list.data_begin(window);
    SpCcl<NumericalRep>::ConstDataIterator it_end = cand_list.data_end();

    // Check output
    int count=0;
    while(it_begin != it_end)
    {
        SCOPED_TRACE("iteration " + std::to_string(count));
        auto slice = *it_begin;
        ASSERT_EQ(static_cast<std::size_t>(slice.size<data::Frequency>()), number_of_channels);
        ASSERT_EQ(static_cast<std::size_t>(slice.size<data::Time>()), expected_spectra_in_block[count]);

        // check the data block start time is as expected
        typename TimeFrequencyType::TimePointType expected_block_start_time(tf_v.front()->start_time() + offset_time + expected_block_start[count]);
        ASSERT_DOUBLE_EQ(expected_block_start_time.time_since_epoch().count(), it_begin.start_time().time_since_epoch().count())
                        << std::setprecision(20) << expected_block_start_time << ", " << it_begin.start_time();

        auto cand_it = it_begin.candidate_begin();
        auto cand_it_end = it_begin.candidate_end();
        ASSERT_EQ(std::distance(cand_it, cand_it_end), expected_candidates_in_block[count]);

        ASSERT_NO_THROW(++it_begin);
        ++count;
    }
    ASSERT_EQ(count, expected_spectra_in_block.size());
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
