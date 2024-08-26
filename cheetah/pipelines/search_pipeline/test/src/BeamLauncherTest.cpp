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
#include "cheetah/pipelines/search_pipeline/test/BeamLauncherTest.h"
#include "cheetah/pipelines/search_pipeline/BeamLauncher.h"
#include "cheetah/data/TimeFrequency.h"
#include "panda/test/TestProducer.h"
#include "cheetah/pipelines/search_pipeline/Empty.h"
#include "cheetah/sigproc/Config.h"
#include "cheetah/sigproc/SigProcFileStream.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


BeamLauncherTest::BeamLauncherTest()
    : ::testing::Test()
{
}

BeamLauncherTest::~BeamLauncherTest()
{
}

void BeamLauncherTest::SetUp()
{
}

void BeamLauncherTest::TearDown()
{
}

class TestConfigModule : public panda::ConfigModule
{
    public:
        TestConfigModule() : panda::ConfigModule("test") {}

    protected:
        void add_options(OptionsDescriptionEasyInit&) {};
};

typedef panda::test::TestProducer<data::TimeFrequency<Cpu, uint8_t>> TestStream;


class TestPipeline : public search_pipeline::PipelineHandler<uint8_t>
{
        typedef search_pipeline::PipelineHandler<uint8_t> BaseT;

    protected:
        volatile unsigned _operator_called;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;

    public:
        TestPipeline(BeamConfig<uint8_t> const& beam_config)
            : BaseT(_cheetah_config, beam_config)
            , _operator_called(0) {}

        void operator()(TimeFrequencyType&) override
        {
            ++_operator_called;
            PANDA_LOG_DEBUG << "Operator called";
        }

        volatile unsigned& operator_called() {
            return _operator_called;
        }

    private:
        CheetahConfig<uint8_t> _cheetah_config;
};

class ExceptionPipeline : public TestPipeline
{
    private:
        unsigned _throw;

    public:
        ExceptionPipeline(BeamConfig<uint8_t> const& beam_config)
            : TestPipeline(beam_config)
            , _throw(0)
        {}

        void operator()(TimeFrequencyType&) override
        {
            ++_operator_called;
            if(_throw==0) { // throw the first time only
                ++_throw;
                throw std::runtime_error("oh oh");
            }
        }

        bool has_thrown() const {
            return _throw > 0;
        }
};

TEST_F(BeamLauncherTest, test_no_beam)
{
    MultiBeamConfig<uint8_t> mb_config;
    TestConfigModule tcm;

    unsigned exec_factory_called = 0;
    auto factory = [&](BeamConfig<uint8_t> const& beam_config)  { ++exec_factory_called; return new TestPipeline(beam_config); };

    unsigned config_factory_called = 0;
    BeamLauncher<TestStream, uint8_t> launcher(mb_config, [&](BeamConfig<uint8_t> const&) -> TestConfigModule const& { ++config_factory_called; return tcm; }, factory );
    ASSERT_EQ(config_factory_called, 0U);

    ASSERT_EQ(0, launcher.exec());
    ASSERT_EQ(exec_factory_called, 0U);
}

TEST_F(BeamLauncherTest, test_single_beam_inactive)
{
    BeamConfig<uint8_t> beam1;
    beam1.active(false);
    MultiBeamConfig<uint8_t> mb_config;
    mb_config.add(beam1);
    TestConfigModule tcm;
    data::TimeFrequency<Cpu, uint8_t> test_data;

    unsigned exec_factory_called = 0;
    auto factory = [&](BeamConfig<uint8_t> const& beam_config)  { ++exec_factory_called; return new TestPipeline(beam_config); };

    unsigned config_factory_called = 0;
    BeamLauncher<TestStream, uint8_t> launcher(mb_config, [&](BeamConfig<uint8_t> const&) -> TestConfigModule const& { ++config_factory_called; return tcm; }, factory );

//  Sending data to the stream
    for (std::uint32_t ii =0 ; ii < launcher.streams().size(); ++ii)
    {
        *(launcher.streams()[ii]) << test_data;
    }

    ASSERT_EQ(config_factory_called, 0U);

    ASSERT_EQ(0, launcher.exec());
    ASSERT_EQ(exec_factory_called, 0U);
}

TEST_F(BeamLauncherTest, test_single_beam_active)
{
    BeamConfig<uint8_t> beam1;
    beam1.active(true);
    MultiBeamConfig<uint8_t> mb_config;
    mb_config.add(beam1);
    TestConfigModule tcm;

    std::shared_ptr<data::TimeFrequency<Cpu,uint8_t>> test_data = std::make_shared<data::TimeFrequency<Cpu, uint8_t>>();

    unsigned exec_factory_called = 0;
    TestPipeline* test_pipeline=nullptr;
    auto factory = [&](BeamConfig<uint8_t> const& beam_config)  { ++exec_factory_called; test_pipeline = new TestPipeline(beam_config); return test_pipeline; };

    unsigned config_factory_called = 0;
    BeamLauncher<TestStream, uint8_t> launcher(mb_config, [&](BeamConfig<uint8_t> const&) -> TestConfigModule const& { ++config_factory_called; return tcm; }, factory );


    ASSERT_EQ(config_factory_called, 1U);
    ASSERT_EQ(exec_factory_called, 1U);


    std::thread th1([&]() {
                launcher.exec();
                });

    volatile bool is_running=launcher.is_running();
    while(!is_running) {
        is_running=launcher.is_running();
    }

     //  Sending data to the stream
    for (std::uint32_t ii = 0 ; ii < launcher.streams().size(); ++ii)
    {
        *(launcher.streams()[ii]) << *test_data;
    }

    ASSERT_NE(nullptr, test_pipeline);
    try
    {
        volatile unsigned op_called;
        do {
            op_called=test_pipeline->operator_called();
        } while(op_called == 0);
        EXPECT_EQ(test_pipeline->operator_called(), 1U);
        launcher.join();
    }
    catch(...)
    {
        PANDA_LOG_DEBUG <<"exception caught";
        launcher.join();
        th1.join();
        throw;
    }
    th1.join();
}

TEST_F(BeamLauncherTest, test_two_beam_active)
{
    BeamConfig<uint8_t> beam1;
    beam1.active(true);
    BeamConfig<uint8_t> beam2;
    beam2.active(true);
    MultiBeamConfig<uint8_t> mb_config;
    mb_config.add(beam1);
    mb_config.add(beam2);
    TestConfigModule tcm;

    data::TimeFrequency<Cpu,uint8_t> test_data;

    unsigned exec_factory_called = 0;
    TestPipeline* test_pipeline=nullptr;
    auto factory = [&](BeamConfig<uint8_t> const& beam_config)  { ++exec_factory_called; test_pipeline = new TestPipeline(beam_config); return test_pipeline; };

    unsigned config_factory_called = 0;
    BeamLauncher<TestStream, uint8_t> launcher(mb_config, [&](BeamConfig<uint8_t> const&) -> TestConfigModule const& { ++config_factory_called; return tcm; }, factory );
    ASSERT_EQ(config_factory_called, 2U);
    ASSERT_EQ(exec_factory_called, 2U);

    //  Sending data to the stream
    for (std::uint32_t ii =0 ; ii < launcher.streams().size(); ++ii)
    {
        *(launcher.streams()[ii]) << test_data;
    }

    std::thread th1([&]() { launcher.exec(); });
    volatile bool is_running=launcher.is_running();
    while(!is_running) {
        is_running=launcher.is_running();
    }

    try
    {
        volatile unsigned op_called=test_pipeline->operator_called();
        while(op_called == 0)
        {
            op_called=test_pipeline->operator_called();
        }
        ASSERT_EQ(test_pipeline->operator_called(), 1U);
        //launcher.join();
    }
    catch(...)
    {
        launcher.join();
        th1.join();
        throw;
    }
    launcher.join();
    th1.join();
}

TEST_F(BeamLauncherTest, test_one_beam_active_one_inactive)
{
    BeamConfig<uint8_t> beam1;
    beam1.active(true);
    BeamConfig<uint8_t> beam2;
    beam2.active(false);
    MultiBeamConfig<uint8_t> mb_config;
    mb_config.add(beam1);
    mb_config.add(beam2);
    TestConfigModule tcm;
    data::TimeFrequency<Cpu,uint8_t> test_data;

    unsigned exec_factory_called = 0;
    TestPipeline* test_pipeline=nullptr;
    auto factory = [&](BeamConfig<uint8_t> const& beam_config)  { ++exec_factory_called; test_pipeline = new TestPipeline(beam_config); return test_pipeline; };

    unsigned config_factory_called = 0;
    BeamLauncher<TestStream, uint8_t> launcher(mb_config, [&](BeamConfig<uint8_t> const&) -> TestConfigModule const& { ++config_factory_called; return tcm; }, factory );
    ASSERT_EQ(config_factory_called, 1U);
    ASSERT_EQ(exec_factory_called, 1U);

//  Sending data to the stream
    for (std::uint32_t ii =0 ; ii < launcher.streams().size(); ++ii)
    {
        *(launcher.streams()[ii]) << test_data;
    }

    std::thread th1([&]() { launcher.exec(); });
    volatile bool is_running=launcher.is_running();
    while(!is_running) {
        is_running=launcher.is_running();
    }

    try
    {
        volatile unsigned op_called;
        do {
            op_called=test_pipeline->operator_called();
        } while(op_called == 0);
        ASSERT_EQ(test_pipeline->operator_called(), 1U);
        launcher.join();
    }
    catch(...)
    {
        launcher.join();
        th1.join();
        throw;
    }
    th1.join();
}

TEST_F(BeamLauncherTest, test_single_beam_pipeline_exception)
{
    BeamConfig<uint8_t> beam1;
    beam1.active(true);
    MultiBeamConfig<uint8_t> mb_config;
    mb_config.add(beam1);
    TestConfigModule tcm;

    std::shared_ptr<data::TimeFrequency<Cpu,uint8_t>> test_data = std::make_shared<data::TimeFrequency<Cpu, uint8_t>>();

    ExceptionPipeline* exception_pipeline=nullptr; // will throw an exception on first call to operatort()
    auto factory = [&](BeamConfig<uint8_t> const& beam_config) { exception_pipeline = new ExceptionPipeline(beam_config); return exception_pipeline; };

    BeamLauncher<TestStream, uint8_t> launcher(mb_config, [&](BeamConfig<uint8_t> const&) -> TestConfigModule const& { return tcm; }, factory );

    std::thread th1([&]() {
                launcher.exec();
                });

    ASSERT_NE(nullptr, exception_pipeline); // sanity check

     //  Sending data to the stream
    for (std::uint32_t ii = 0 ; ii < launcher.streams().size(); ++ii)
    {
        *(launcher.streams()[ii]) << *test_data;
    }

    try
    {
        volatile unsigned ex_op;
        do {
            ex_op = exception_pipeline->operator_called();
        } while(ex_op == 0);
        ASSERT_EQ(exception_pipeline->operator_called(), 1U);
        ASSERT_TRUE(exception_pipeline->has_thrown());
        launcher.join();
    }
    catch(...)
    {
        PANDA_LOG_DEBUG << "unexpected exception thrown - cleaning up test";
        launcher.join();
        th1.join();
        throw;
    }
    th1.join();
}


} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
