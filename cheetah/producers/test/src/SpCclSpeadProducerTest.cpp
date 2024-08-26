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
#include "cheetah/producers/test/SpCclSpeadProducerTest.h"
#include "cheetah/io/exporters/SpCclSpeadStreamerConfig.h"
#include "cheetah/producers/SpCclSpeadProducer.h"
#include "cheetah/io/exporters/test/SpCclSpeadStreamerTest.h"
#include "cheetah/io/exporters/SpCclSpeadStreamer.h"
#include "cheetah/io/exporters/SpCclSpeadReader.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include <panda/ProcessingEngine.h>
#include <panda/ProcessingEngineConfig.h>
#include <vector>


namespace ska {
namespace cheetah {
namespace producers {
namespace test {

template<typename T>
SpCclSpeadProducerTest<T>::SpCclSpeadProducerTest()
    : ::testing::Test()
{
}

TYPED_TEST(SpCclSpeadProducerTest, test_instantiation)
{
    typedef TypeParam TimeFrequencyType;
    typedef SpCclSpeadProducer<TimeFrequencyType> ProducerTestType;
    typedef ska::panda::DataManager<ProducerTestType> DataManagerType;

    // setup the consumer side
    io::exporters::SpCclSpeadReaderConfig reader_config;
    ProducerTestType producer(reader_config);
    DataManagerType data_manager(producer);
}

TYPED_TEST(SpCclSpeadProducerTest, test_single_candidate_reception)
{
    typedef TypeParam TimeFrequencyType;
    typedef typename TimeFrequencyType::NumericalRep NumericalRep;
    typedef data::SpCcl<NumericalRep> SpCclType;
    typedef typename SpCclType::SpCandidateType Candidate;

    typedef SpCclSpeadProducer<TimeFrequencyType> ProducerTestType;
    typedef ska::panda::DataManager<ProducerTestType> DataManagerType;
    typedef typename ProducerTestType::SpCclType SpCandidateDataType;

    io::exporters::SpCclSpeadReaderConfig reader_config;
    ProducerTestType producer(reader_config);
    DataManagerType data_manager(producer);


    // set up the data
    typename utils::ModifiedJulianClock::time_point start_time(utils::julian_day(2458179.500000));

    // Define a BlocksType called blocks
    typename SpCclType::BlocksType blocks;

    // Generate a TF block
    std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(1000)
                                                       , data::DimensionSize<data::Frequency>(10));

    // Set start time of block
    block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));

    // Set sample interval
    block->sample_interval(0.001 * boost::units::si::seconds);

    // Add block to blocks
    blocks.push_back(block);
    blocks.push_back(std::make_shared<TimeFrequencyType>( data::DimensionSize<data::Time>(100)
                                                        , data::DimensionSize<data::Frequency>(10)));

    std::shared_ptr<data::SpCcl<NumericalRep>> data(new data::SpCcl<NumericalRep>(blocks, data::DimensionIndex<data::Time>(0)));

    // Set duration of candidate in bins
    std::size_t cand_duration = 40;

    // Set an end time
    typename Candidate::MsecTimeType cand1_tend(cand_duration * block->sample_interval());

    // Set a DM
    typename Candidate::Dm cand1_dm(00.0 * data::parsecs_per_cube_cm);

    // Set other candidate parameters
    Candidate candidate_1( cand1_dm
                        , typename Candidate::MsecTimeType(0.0 * boost::units::si::seconds)
                        , typename Candidate::MsecTimeType(0.001 * boost::units::si::seconds)
                        , cand1_tend
                        , 2.0
                        );

    data->push_back(candidate_1);

    ska::panda::ProcessingEngine engine(1);
    ska::cheetah::io::exporters::SpCclSpeadStreamerConfig config;
    io::exporters::SpCclSpeadStreamer writer(config, engine);
    writer << *data;

    std::tuple<std::shared_ptr<SpCandidateDataType>> received_data_tuple = data_manager.next();
    SpCandidateDataType const& received_data = *std::get<0>(received_data_tuple);

    // Test data block
    ASSERT_LE(received_data.data_size(), data->tf_blocks()[0]->data_size());
    ASSERT_EQ(received_data.number_of_channels(), block->number_of_channels());
    ASSERT_EQ(received_data.sample_interval(), block->sample_interval());
    ASSERT_EQ(received_data.number_of_spectra(), cand_duration);

    // Test candidate parameters
    ASSERT_EQ(received_data.number_of_candidates(), data->size());
    ASSERT_EQ(received_data.candidate(0).dm(), (*data)[0].dm());
    ASSERT_EQ((received_data).candidate(0).sigma(), (*data)[0].sigma());
    ASSERT_EQ(received_data.candidate(0).width(), (*data)[0].width());
    ASSERT_EQ(received_data.candidate(0).duration(), (*data)[0].tend());
    ASSERT_EQ(received_data.candidate(0).start_time(), block->start_time());
}


TYPED_TEST(SpCclSpeadProducerTest, test_multiple_candidate_reception)
{
    typedef TypeParam TimeFrequencyType;
    typedef typename TimeFrequencyType::NumericalRep NumericalRep;
    typedef data::SpCcl<NumericalRep> SpCclType;
    typedef typename SpCclType::SpCandidateType Candidate;

    typedef SpCclSpeadProducer<TimeFrequencyType> ProducerTestType;
    typedef ska::panda::DataManager<ProducerTestType> DataManagerType;
    typedef typename ProducerTestType::SpCclType SpCandidateDataType;

    io::exporters::SpCclSpeadReaderConfig reader_config;
    ProducerTestType producer(reader_config);
    DataManagerType data_manager(producer);

    // set up the data
    typename utils::ModifiedJulianClock::time_point start_time(utils::julian_day(2458179.500000));

    // Define a BlocksType called blocks
    typename SpCclType::BlocksType blocks;

    // Generate a TF block
    std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(1000)
                                                       , data::DimensionSize<data::Frequency>(10));

    // Set start time of block
    block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));

    // Set sample interval
    block->sample_interval(0.001 * boost::units::si::seconds);

    // Add block to blocks
    blocks.push_back(block);
    blocks.push_back(std::make_shared<TimeFrequencyType>( data::DimensionSize<data::Time>(100)
                                                        , data::DimensionSize<data::Frequency>(10)));


    std::shared_ptr<TimeFrequencyType> block_2 = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(600)
                                                       , data::DimensionSize<data::Frequency>(10));

    block_2->start_time(block->end_time() + block->sample_interval());
    block_2->sample_interval(0.001 * boost::units::si::seconds);
    blocks.push_back(block_2);

    std::shared_ptr<data::SpCcl<NumericalRep>> data(new data::SpCcl<NumericalRep>(blocks, data::DimensionIndex<data::Time>(0)));

    std::size_t cand_duration = 40; // number of bins candidate 1 covers

    // Set an end time for candidate 1
    typename Candidate::MsecTimeType cand1_tend(cand_duration * block->sample_interval());

    // Set a DM for candidate 1
    typename Candidate::Dm cand1_dm(00.0 * data::parsecs_per_cube_cm);

    // Set other candidate parameters
    Candidate candidate_1( cand1_dm
                        , typename Candidate::MsecTimeType(0.0 * boost::units::si::seconds)
                        , typename Candidate::MsecTimeType(0.001 * boost::units::si::seconds)
                        , cand1_tend
                        , 2.0
                        );

    data->push_back(candidate_1);

    std::size_t cand2_duration = 20; // number of bins candidate 2 covers

    // Set an end time for candidate 2
    typename Candidate::MsecTimeType cand2_tend(cand2_duration * block->sample_interval());

    // Set a DM for candidate 2
    typename Candidate::Dm cand2_dm(10.0 * data::parsecs_per_cube_cm);

    // Set other candidate parameters
    Candidate candidate_2( cand2_dm
                        , cand1_tend - static_cast<typename Candidate::MsecTimeType>(block->sample_interval())
                        , typename Candidate::MsecTimeType(0.001 * boost::units::si::seconds)
                        , cand1_tend + static_cast<typename Candidate::MsecTimeType>(block->sample_interval())
                        , 2.0
                        );

    data->push_back(candidate_2);

    ska::panda::ProcessingEngine engine(1);
    ska::cheetah::io::exporters::SpCclSpeadStreamerConfig config;
    io::exporters::SpCclSpeadStreamer writer(config, engine);
    writer << *data;

    std::tuple<std::shared_ptr<SpCandidateDataType>> received_data_tuple = data_manager.next();
    SpCandidateDataType const& received_data = *std::get<0>(received_data_tuple);

    // Test data block
    ASSERT_LE(received_data.data_size(), data->tf_blocks()[0]->data_size());
    ASSERT_EQ(received_data.number_of_channels(), block->number_of_channels());
    ASSERT_EQ(received_data.sample_interval(), block->sample_interval());
    ASSERT_EQ(received_data.number_of_spectra(), cand_duration + 1);

    // Test candidate data
    ASSERT_EQ(received_data.number_of_candidates(), data->size());
    for(std::size_t i=0; i< received_data.number_of_candidates(); ++i) {
        ASSERT_EQ(received_data.candidate(i).dm(), (*data)[i].dm());
        ASSERT_EQ(received_data.candidate(i).sigma(), (*data)[i].sigma());
        ASSERT_EQ(received_data.candidate(i).width(), (*data)[i].width());
        ASSERT_EQ(received_data.candidate(i).duration(), (*data)[i].tend());
    }
    ASSERT_EQ(received_data.candidate(0).start_time(), block->start_time());
    ASSERT_EQ(received_data.candidate(1).start_time(), data->start_time(candidate_2));
}

} // namespace test
} // namespace producers
} // namespace cheetah
} // namespace ska
