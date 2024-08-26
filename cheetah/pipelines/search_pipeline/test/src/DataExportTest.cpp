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
#include "cheetah/io/exporters/SpCclFileStreamerConfig.h"
#include "cheetah/io/exporters/SpCclSigProc.h"
#include "cheetah/pipelines/search_pipeline/test/DataExportTest.h"
#include "cheetah/pipelines/search_pipeline/DataExport.h"
#include <panda/test/TestDir.h>


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


DataExportTest::DataExportTest()
    : ::testing::Test()
{
}

DataExportTest::~DataExportTest()
{
}

void DataExportTest::SetUp()
{
}

void DataExportTest::TearDown()
{
}

// wrapper to monitor calls to the operator()
template<class Sink>
struct SinkWrapper : public Sink
{
    public:
        template<typename... Args>
        SinkWrapper(Args&&... args)
            : Sink(std::forward<Args>(args)...)
            , _test_call_count(0)
        {
        }

        template<typename Ret, typename... Args>
        Ret operator()(Args&&... args) {
            ++_test_call_count;
            return Sink::operator()(std::forward<Args>(args)...);
        }

        unsigned call_count() const {
            return _test_call_count;
        }

    private:
        unsigned _test_call_count = 0;

};

TEST_F(DataExportTest, test_sink_spccl)
{
    panda::test::TestDir dir;
    dir.create();
    io::exporters::DataExportConfig config;
    io::exporters::SpCclFileStreamerConfig sink_config;
    sink_config.dir(dir.path());
    std::string const sink_id = sink_config.id();
    config.add_sink(sink_config);

    // set up channels
    panda::ChannelId channel_id("active_channel_spccl");
    panda::ChannelInfo channel_info(channel_id);
    channel_info.activate(true);
    channel_info.sink(sink_id);
    config.channel(channel_info);

    panda::ChannelId inactive_channel_id("inactive_channel_spccl");
    panda::ChannelInfo inactive_channel_info(inactive_channel_id);
    inactive_channel_info.activate(false);
    inactive_channel_info.sink(sink_id);
    config.channel(inactive_channel_info);

    search_pipeline::DataExport<uint8_t> exporter(config);

    // set up spccl config node
    std::shared_ptr<data::SpCcl<uint8_t>> data = data::SpCcl<uint8_t>::make_shared();
    ASSERT_TRUE(exporter.is_active<data::SpCcl<uint8_t>>(channel_id));
    exporter.send(channel_id, data);

    ASSERT_FALSE(exporter.is_active<data::SpCcl<uint8_t>>(inactive_channel_id));
    exporter.send(inactive_channel_id, data);
}

TEST_F(DataExportTest, test_sink_spccl_sigproc_with_data)
{
    panda::test::TestDir dir;
    dir.create();
    io::exporters::DataExportConfig config;
    io::exporters::SpCclSigProcConfig sink_config;
    sink_config.dir(dir.path().string());
    std::string const sink_id = sink_config.id();
    config.add_sink(sink_config);

    //set up channels
    panda::ChannelId channel_id("active_channel_spccl_sigproc");
    panda::ChannelInfo channel_info(channel_id);
    channel_info.activate(true);
    channel_info.sink(sink_id);
    config.channel(channel_info);

    search_pipeline::DataExport<uint8_t> exporter(config);

// Generating data
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

    ASSERT_TRUE(exporter.is_active<data::SpCcl<uint8_t>>(channel_id));
    exporter.send(channel_id, data::SpCcl<uint8_t>::make_shared(d1));


    panda::ChannelId inactive_channel_id("inactive_channel_spccl_sigproc");
    panda::ChannelInfo inactive_channel_info(inactive_channel_id);
    inactive_channel_info.activate(false);
    inactive_channel_info.sink(sink_id);
    config.channel(inactive_channel_info);

    ASSERT_FALSE(exporter.is_active<data::SpCcl<uint8_t>>(inactive_channel_id));
    exporter.send(inactive_channel_id, data::SpCcl<uint8_t>::make_shared(d1));
}

TEST_F(DataExportTest, test_spccl_sigproc_sink_without_data)
{
    panda::test::TestDir dir;
    dir.create();
    io::exporters::DataExportConfig config;
    io::exporters::SpCclSigProcConfig sink_config;
    sink_config.dir(dir.path().string());
    std::string const sink_id = sink_config.id();
    config.add_sink(sink_config);

    //set up channels
    panda::ChannelId channel_id("active_channel_spccl_sigproc");
    panda::ChannelInfo channel_info(channel_id);
    channel_info.activate(true);
    channel_info.sink(sink_id);
    config.channel(channel_info);

    search_pipeline::DataExport<uint8_t> exporter(config);

    std::shared_ptr<data::SpCcl<uint8_t>> data = data::SpCcl<uint8_t>::make_shared();
    ASSERT_TRUE(exporter.is_active<data::SpCcl<uint8_t>>(channel_id));
    exporter.send(channel_id, data);


    panda::ChannelId inactive_channel_id("inactive_channel_spccl_sigproc");
    panda::ChannelInfo inactive_channel_info(inactive_channel_id);
    inactive_channel_info.activate(false);
    inactive_channel_info.sink(sink_id);
    config.channel(inactive_channel_info);

    ASSERT_FALSE(exporter.is_active<data::SpCcl<uint8_t>>(inactive_channel_id));
    exporter.send(inactive_channel_id, data);


}


} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
