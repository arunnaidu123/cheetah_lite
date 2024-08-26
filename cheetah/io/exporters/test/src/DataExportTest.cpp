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
#include "cheetah/io/exporters/test/DataExportTest.h"
#include "cheetah/io/exporters/DataExport.h"
#include "panda/test/TestChunk.h"
#include "panda/test/TestChunkHandler.h"
#include "panda/ProcessingEngineConfig.h"
#include <iostream>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
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

/*
 * a DataExport
 * that exposes the protected members so we can test easily
 */
template<typename... TestChunks>
class TestDataExport : public DataExport<TestChunks...>
{
        typedef DataExport<TestChunks...> BaseT;

    public:
        TestDataExport(DataExportConfig& config)
            : BaseT(config) {}
        void init() { BaseT::init(); }

        template<typename DataType>
        void set_factory(ExporterType const& type_id, typename ExportInitialiser<BaseT, DataType>::FactoryType const& factory)
        {
            BaseT::template set_factory<DataType>(type_id, factory);
        }
};

TEST_F(DataExportTest, test_config_not_set)
{
    panda::ChannelId channel_id("channel_1");
    ExporterType exporter_type("test_exporter");

    DataExportConfig config;
    panda::ProcessingEngineConfig engine_config;
    engine_config.number_of_threads(0);
    config.set_engine_config(engine_config);

    TestDataExport<panda::test::TestChunk_A> exporter(config);

    bool factory_called = false;
    panda::test::TestChunkHandler<panda::test::TestChunk_A> handler(false); // this should never be run
    exporter.set_factory<panda::test::TestChunk_A>(exporter_type, [&handler, &factory_called](exporters::DataExportStreamConfig) { factory_called=true; return handler; } );
    exporter.init();
    ASSERT_FALSE(factory_called);

    // verify available() is returning the correct value
    std::set<ExporterType> expected_types({exporter_type});
    ASSERT_EQ(expected_types, exporter.available());

    // check channel is not calling any handler
    std::shared_ptr<panda::test::TestChunk_A> chunk = std::make_shared<panda::test::TestChunk_A>("ooo");
    exporter.send(channel_id, chunk);
    ASSERT_FALSE(handler.executed());
}

class TestExporterConfig : public utils::Config {
    public:
        TestExporterConfig() : utils::Config("test") {}
        void add_options(OptionsDescriptionEasyInit&) {}
};

TEST_F(DataExportTest, test_config_set_single_thread)
{
    /**
     * @test
     *    specify configuration for a single exporter, assigned to a channel. Any data sent to the channel should be received by the handler
     *    syncronous engine
     */
    panda::ChannelId channel_id("channel_1");
    ExporterType exporter_type("test_exporter");
    TestExporterConfig exporter_config;

    DataExportConfig config;
    panda::ProcessingEngineConfig engine_config;
    engine_config.number_of_threads(0);
    config.set_engine_config(engine_config);
    config.activate(channel_id);
    DataExportStreamConfig export_config(channel_id, config.engine(channel_id), exporter_type, exporter_config);
    config.add_exporter(export_config);

    TestDataExport<panda::test::TestChunk_A> exporter(config);

    panda::test::TestChunkHandler<panda::test::TestChunk_A> handler(false);
    bool factory_called = false;
    exporter.set_factory<panda::test::TestChunk_A>(exporter_type,
                                                  [&handler, &factory_called](exporters::DataExportStreamConfig)
                                                  {
                                                      factory_called=true;
                                                      return handler;
                                                  } );
    exporter.init();
    ASSERT_TRUE(factory_called);

    // check channel is calling our handler
    std::shared_ptr<panda::test::TestChunk_A> chunk = std::make_shared<panda::test::TestChunk_A>("ooo");
    exporter.send(channel_id, chunk);

    ASSERT_TRUE(handler.executed());
    ASSERT_EQ(*chunk, handler.chunk());
}

TEST_F(DataExportTest, test_config_set_multi_thread)
{
    /**
     * @test
     *    specify configuration for a single exporter, assigned to a channel. Any data sent to the channel should be received by the handler
     *    multi-threaded engine
     */
    panda::ChannelId channel_id("channel_1");
    ExporterType exporter_type("test_exporter");
    TestExporterConfig exporter_config;

    DataExportConfig config;
    config.number_of_threads(1);
    panda::Engine engine;
    DataExportStreamConfig export_config(channel_id, engine, exporter_type, exporter_config);
    config.add_exporter(export_config);
    config.activate(channel_id);

    TestDataExport<panda::test::TestChunk_A> exporter(config);

    bool factory_called = false;
    panda::test::TestChunkHandler<panda::test::TestChunk_A> handler(true);
    exporter.set_factory<panda::test::TestChunk_A>(exporter_type, [&handler, &factory_called](exporters::DataExportStreamConfig) { factory_called=true; return handler; } );
    exporter.init();
    ASSERT_TRUE(factory_called);

    // check channel is calling our handler
    std::shared_ptr<panda::test::TestChunk_A> chunk = std::make_shared<panda::test::TestChunk_A>("ooo");
    exporter.send(channel_id, chunk);
    handler.wait();
    ASSERT_TRUE(handler.executed());
    ASSERT_EQ(*chunk, handler.chunk());
}

TEST_F(DataExportTest, test_activate_test_probe)
{
    panda::ChannelId channel_id("channel_1");
    DataExportConfig config;
    config.number_of_threads(0);
    TestDataExport<panda::test::TestChunk_A> exporter(config);

    // activate the probe
    TestProbe<panda::test::TestChunk_A>& probe = exporter.activate_test_probe<panda::test::TestChunk_A>(channel_id);
    std::shared_ptr<panda::test::TestChunk_A> chunk = std::make_shared<panda::test::TestChunk_A>("ooo");
    exporter.send(channel_id, chunk);
    ASSERT_EQ(&*probe.data(), &*chunk);
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
