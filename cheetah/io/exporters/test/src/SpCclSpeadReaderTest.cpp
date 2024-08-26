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
#include "cheetah/io/exporters/test/SpCclSpeadReaderTest.h"
#include "cheetah/io/exporters/SpCclSpeadReader.h"
#include "cheetah/io/exporters/SpCclSpeadStreamer.h"
#include <memory>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


SpCclSpeadReaderTest::SpCclSpeadReaderTest()
    : ::testing::Test()
{
}

SpCclSpeadReaderTest::~SpCclSpeadReaderTest()
{
}

void SpCclSpeadReaderTest::SetUp()
{
}

void SpCclSpeadReaderTest::TearDown()
{
}

template<typename NumericalRep>
std::shared_ptr<data::SpCcl<NumericalRep>> spccl_data()
{
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
    return data;
}

/*
 *  The client should listen until it is stopped by the owner
 *  Spead2 allows the send side to emiat an end of stream marker.
 *  We use this marker to reset the stream so it is cabable of receiving
 *  any new data.
 */
TEST_F(SpCclSpeadReaderTest, test_reset)
{
    typedef uint8_t NumericalRep;

    SpCclSpeadStreamerConfig writer_config;
    SpCclSpeadReaderConfig reader_config;
    panda::ProcessingEngine engine(2);

    // generate some data
    auto data = spccl_data<NumericalRep>();
    typedef typename std::remove_reference<decltype(*data)>::type::TimeFrequencyType TimeFrequencyType;

    std::vector<std::shared_ptr<data::SpCandidateData<TimeFrequencyType>>> recv_data;
    SpCclSpeadReader<TimeFrequencyType> reader(reader_config
                               , [&](std::shared_ptr<data::SpCandidateData<TimeFrequencyType>> const& d)
                                 {
                                    std::cout << "hello" << std::endl;
                                    recv_data.push_back(d);
                                 }
                               );
    bool stopped = false;
    reader.add_stop_callback([&]() { stopped = true; });
    reader.start();
    bool started = false;
    reader.add_start_callback([&]() { started = true; });
    {
        exporters::SpCclSpeadStreamer writer(writer_config, engine);
        writer << *data;
    }
    // wait for us to get data before sending more data.
    while(!(stopped && started)) {
        reader_config.engine().poll_one();
    }
    {
        exporters::SpCclSpeadStreamer writer(writer_config, engine);
        writer << *data;
    }
    // we expect to get all our data despite the reset
    while(recv_data.size() < 2 ) {
        reader_config.engine().poll_one();
    }
    // verify data transfered correctly
    //for(auto const& d : recv_data) {
        //ASSERT_TRUE((*d) == *data);
    //}
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
