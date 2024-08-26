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
#include "cheetah/io/exporters/test/SpCclSpeadStreamerTest.h"
#include "cheetah/io/exporters/SpCclSpeadStreamer.h"
#include "cheetah/io/exporters/SpCclSpeadReader.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include <vector>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


template<typename T>
SpCclSpeadStreamerTest<T>::SpCclSpeadStreamerTest()
    : ::testing::Test()
{
}

template<typename T>
SpCclSpeadStreamerTest<T>::~SpCclSpeadStreamerTest()
{
}

template<typename T>
void SpCclSpeadStreamerTest<T>::SetUp()
{
}

template<typename T>
void SpCclSpeadStreamerTest<T>::TearDown()
{
}

template<typename TimeFrequencyType>
class TestExporter {
    public:
        TestExporter(SpCclSpeadStreamerConfig const& config, SpCclSpeadReaderConfig const& reader_config)
            : _engine(1)
            , _writer(config, _engine)
            , _reader_config(reader_config)
            , _reader(reader_config, [this](std::shared_ptr<data::SpCandidateData<TimeFrequencyType>> const& d)
                                     {
                                        _rdata.push_back(d);
                                     }
                     )
            , _expected(1)
        {
            _reader.start();
        }

        template<typename T>
        TestExporter& operator<<(data::SpCcl<T> const& data) {
            _writer << data;
            return *this;
        }

        // returns the next received chunk, blocking
        data::SpCandidateData<TimeFrequencyType> const& get()
        {
            // wait for data transfer
            while(_rdata.size() < _expected || _rdata[_expected-1].get() == nullptr) {
                _reader_config.engine().poll_one();
            }
            ++_expected;
            return *_rdata[_expected -2];
        }

    private:
        panda::ProcessingEngine _engine;
        exporters::SpCclSpeadStreamer _writer;
        SpCclSpeadReaderConfig const& _reader_config;
        SpCclSpeadReader<TimeFrequencyType, std::function<void(std::shared_ptr<data::SpCandidateData<TimeFrequencyType>> const&)>> _reader;
        std::vector<std::shared_ptr<data::SpCandidateData<TimeFrequencyType>>> _rdata;
        std::size_t _expected;
};

TYPED_TEST(SpCclSpeadStreamerTest, test_construct_destruct_without_using)
{
    panda::ProcessingEngine engine(1);
    SpCclSpeadStreamerConfig config;
    exporters::SpCclSpeadStreamer writer(config, engine);
}

TYPED_TEST(SpCclSpeadStreamerTest, test_send_single_candidate)
{
    typedef TypeParam NumericalRep;
    typedef data::SpCcl<NumericalRep> SpCclType;
    typedef typename SpCclType::TimeFrequencyType TimeFrequencyType;
    typedef typename SpCclType::SpCandidateType Candidate;

    // set up the data
    typename utils::ModifiedJulianClock::time_point start_time(utils::julian_day(2458179.500000));

    typename SpCclType::BlocksType blocks;
    std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(1000)
                                                       , data::DimensionSize<data::Frequency>(10));

    block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));
    block->sample_interval(0.001 * boost::units::si::seconds);
    blocks.push_back(block);
    blocks.push_back(std::make_shared<TimeFrequencyType>( data::DimensionSize<data::Time>(100)
                                                        , data::DimensionSize<data::Frequency>(10)));

    std::shared_ptr<data::SpCcl<NumericalRep>> data(new data::SpCcl<NumericalRep>(blocks
                                                                                 , data::DimensionIndex<data::Time>(0)));

    std::size_t cand_duration = 40; // number of bins candidate covers
    typename Candidate::MsecTimeType cand1_tend(cand_duration * block->sample_interval());
    typename Candidate::Dm cand1_dm(00.0 * data::parsecs_per_cube_cm);
    Candidate candidate_1( cand1_dm
                        , typename Candidate::MsecTimeType(0.0 * boost::units::si::seconds)
                        , typename Candidate::MsecTimeType(0.001 * boost::units::si::seconds)
                        , cand1_tend
                        , 2.0
                        );
    data->push_back(candidate_1);

    // send and recieve the data
    SpCclSpeadStreamerConfig config;
    SpCclSpeadReaderConfig reader_config;
    TestExporter<TimeFrequencyType> exporter(config, reader_config);
    exporter << *data;

    auto const& rdata = exporter.get();

    // verify reception
    // data
    ASSERT_LE(rdata.data_size(), data->tf_blocks()[0]->data_size());
    ASSERT_EQ(rdata.number_of_channels(), block->number_of_channels());
    ASSERT_EQ(rdata.sample_interval(), block->sample_interval());
    ASSERT_EQ(rdata.number_of_spectra(), cand_duration);

    // candidates
    ASSERT_EQ(rdata.number_of_candidates(), data->size());
    for(std::size_t i=0; i< rdata.number_of_candidates(); ++i) {
        ASSERT_EQ((rdata).candidate(i).dm(), (*data)[i].dm());
        ASSERT_EQ((rdata).candidate(i).sigma(), (*data)[i].sigma());
        ASSERT_EQ((rdata).candidate(i).width(), (*data)[i].width());
        ASSERT_EQ((rdata).candidate(i).duration(), (*data)[i].tend());
    }
    ASSERT_EQ((rdata).candidate(0).start_time(), block->start_time());
}

TYPED_TEST(SpCclSpeadStreamerTest, test_send_single_candidate_across_blocks)
{
    typedef TypeParam NumericalRep;
    typedef data::SpCcl<NumericalRep> SpCclType;
    typedef typename SpCclType::TimeFrequencyType TimeFrequencyType;
    typedef typename SpCclType::SpCandidateType Candidate;

    // set up the data
    typename utils::ModifiedJulianClock::time_point start_time(utils::julian_day(2458179.500000));

    typename SpCclType::BlocksType blocks;
    std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(1000)
                                                       , data::DimensionSize<data::Frequency>(10));

    block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));
    block->sample_interval(0.001 * boost::units::si::seconds);
    blocks.push_back(block);
    std::shared_ptr<TimeFrequencyType> block_2 = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(100)
                                                       , data::DimensionSize<data::Frequency>(10));

    block_2->start_time(block->end_time() + block->sample_interval());
    block_2->sample_interval(0.001 * boost::units::si::seconds);
    blocks.push_back(block_2);

    std::shared_ptr<data::SpCcl<NumericalRep>> data(new data::SpCcl<NumericalRep>(blocks
                                                                                  , data::DimensionIndex<data::Time>(0)));

    std::size_t cand_duration = block->number_of_spectra() + 10; // number of bins candidate covers
    typename Candidate::MsecTimeType cand1_tstart(10.0 * block->sample_interval());
    typename Candidate::MsecTimeType cand1_tend(cand_duration * block->sample_interval()); cand1_tend += cand1_tstart;
    typename Candidate::Dm cand1_dm(100.0 * data::parsecs_per_cube_cm);
    Candidate candidate_1( cand1_dm
                         , cand1_tstart
                         , typename Candidate::MsecTimeType(0.1 * boost::units::si::seconds)
                         , cand1_tend
                         , 2.0
                         );
    data->push_back(candidate_1);

    SpCclSpeadStreamerConfig config;
    SpCclSpeadReaderConfig reader_config;
    TestExporter<TimeFrequencyType> exporter(config, reader_config);
    exporter << *data;

    auto const& rdata = exporter.get();

    // verify reception
    // data
    ASSERT_EQ(rdata.number_of_channels(), block->number_of_channels());
    ASSERT_EQ(rdata.sample_interval(), block->sample_interval());
    ASSERT_EQ(rdata.number_of_spectra(), cand_duration);

}

TYPED_TEST(SpCclSpeadStreamerTest, test_send_overlapping_candidates_in_first_block)
{
    typedef TypeParam NumericalRep;
    typedef data::SpCcl<NumericalRep> SpCclType;
    typedef typename SpCclType::TimeFrequencyType TimeFrequencyType;
    typedef typename SpCclType::SpCandidateType Candidate;

    // set up the data
    typename utils::ModifiedJulianClock::time_point start_time(utils::julian_day(2458179.500000));

    typename SpCclType::BlocksType blocks;
    std::shared_ptr<TimeFrequencyType> block = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(1000)
                                                       , data::DimensionSize<data::Frequency>(10));

    block->start_time(static_cast<typename TimeFrequencyType::TimePointType>(start_time));
    block->sample_interval(0.001 * boost::units::si::seconds);
    blocks.push_back(block);
    std::shared_ptr<TimeFrequencyType> block_2 = std::make_shared<TimeFrequencyType>(
                                                         data::DimensionSize<data::Time>(600)
                                                       , data::DimensionSize<data::Frequency>(10));

    block_2->start_time(block->end_time() + block->sample_interval());
    block_2->sample_interval(0.001 * boost::units::si::seconds);
    blocks.push_back(block_2);

    std::shared_ptr<data::SpCcl<NumericalRep>> data(new data::SpCcl<NumericalRep>(blocks
                                                                                 , data::DimensionIndex<data::Time>(0)));

    std::size_t cand_duration = 40; // number of bins candidate covers
    typename Candidate::MsecTimeType cand1_tend(cand_duration * block->sample_interval());
    typename Candidate::Dm cand1_dm(10.0 * data::parsecs_per_cube_cm);
    Candidate candidate_1( cand1_dm
                        , typename Candidate::MsecTimeType(0.0 * boost::units::si::seconds)
                        , typename Candidate::MsecTimeType(0.001 * boost::units::si::seconds)
                        , cand1_tend
                        , 2.0
                        );
    data->push_back(candidate_1);
    // candidate 2 starts before the cand1_tend by a single sample and extends to 1 sample beyond it
    Candidate candidate_2( cand1_dm
                        , cand1_tend - static_cast<typename Candidate::MsecTimeType>(block->sample_interval())
                        , typename Candidate::MsecTimeType(0.001 * boost::units::si::seconds)
                        , cand1_tend + static_cast<typename Candidate::MsecTimeType>(block->sample_interval())
                        , 2.0
                        );
    data->push_back(candidate_2);

    // send and recieve the data
    SpCclSpeadStreamerConfig config;
    SpCclSpeadReaderConfig reader_config;
    TestExporter<TimeFrequencyType> exporter(config, reader_config);
    exporter << *data;

    auto const& rdata = exporter.get();

    // verify reception
    // data
    ASSERT_EQ(rdata.number_of_channels(), block->number_of_channels());
    ASSERT_LE(rdata.data_size(), data->tf_blocks()[0]->data_size());
    ASSERT_EQ(rdata.sample_interval(), block->sample_interval());
    ASSERT_EQ(rdata.number_of_spectra(), cand_duration + 1); // enough to cover both candidates

    // candidates
    ASSERT_EQ(rdata.number_of_candidates(), data->size());
    for(std::size_t i=0; i< rdata.number_of_candidates(); ++i) {
        ASSERT_EQ(rdata.candidate(i).dm(), (*data)[i].dm());
        ASSERT_EQ(rdata.candidate(i).sigma(), (*data)[i].sigma());
        ASSERT_EQ(rdata.candidate(i).width(), (*data)[i].width());
        ASSERT_EQ(rdata.candidate(i).duration(), (*data)[i].tend());
    }
    ASSERT_EQ(rdata.candidate(0).start_time(), block->start_time());
    ASSERT_EQ(rdata.candidate(1).start_time(), data->start_time(candidate_2));
}



} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
