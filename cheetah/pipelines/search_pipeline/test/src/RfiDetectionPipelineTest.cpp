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
#include "cheetah/pipelines/search_pipeline/test/RfiDetectionPipelineTest.h"
#include "cheetah/pipelines/search_pipeline/RfiDetectionPipeline.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include <mutex>
#include <condition_variable>


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


RfiDetectionPipelineTest::RfiDetectionPipelineTest()
    : ::testing::Test()
{
}

RfiDetectionPipelineTest::~RfiDetectionPipelineTest()
{
}

void RfiDetectionPipelineTest::SetUp()
{
}

void RfiDetectionPipelineTest::TearDown()
{
}

template<typename NumericalT>
struct RfiTests
{
    typedef typename data::TimeFrequency<Cpu, NumericalT> TimeFrequencyType;
    typedef typename TimeFrequencyType::TimePointType TimePointType;
    typedef typename TimeFrequencyType::DataType DataType;

    struct TestOutputHandler {
        TestOutputHandler()
            : _count(0) {}

        template<typename TimeFrequencyT>
        void operator()(std::shared_ptr<TimeFrequencyT>& data) {
            this->operator()(*data);
        }

        template<typename TimeFrequencyT>
        void operator()(TimeFrequencyT& data) {
            ASSERT_LT(_start_time, data.start_time());
            _start_time = data.start_time();
            std::lock_guard<std::mutex> lk(_mutex);
            ++_count;
            _cv.notify_all();
            PANDA_LOG_DEBUG << "test handler called: count=" << _count;
        }

        void wait(std::size_t count) {
            std::unique_lock<std::mutex> lk(_mutex);
            _cv.wait(lk, [this, count]() { return _count == count; });
        }

        private:
            TimePointType _start_time;
            std::mutex _mutex;
            std::condition_variable _cv;
            std::size_t _count;
    };

    static void run()
    {

        CheetahConfig<NumericalT> config;
        auto f_low =  data::TimeFrequency<Cpu, uint8_t>::FrequencyType(1.2 * boost::units::si::giga * boost::units::si::hertz);
        auto f_high =  data::TimeFrequency<Cpu, uint8_t>::FrequencyType(1.8 * boost::units::si::giga * boost::units::si::hertz);
        typename data::TimeFrequency<Cpu, NumericalT>::TimeType t_samp(64.0 * boost::units::si::micro * data::seconds);
        data::DimensionSize<data::Time> number_of_samples(1<<15);
        data::DimensionSize<data::Frequency> number_of_channels(1024);
        typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
        auto delta = (f_low - f_high)/ (double)number_of_channels;

        //Set up noise parameters for data to be passed through
        //the pipeline
        generators::GaussianNoiseConfig noise_config;
        noise_config.mean(96.0);
        noise_config.std_deviation(10.0);
        generators::GaussianNoise<data::TimeFrequency<Cpu, uint8_t>> noise(noise_config);
        BeamConfigType<uint8_t> beam_config(config.pool_manager());

        RfiDetectionPipeline<NumericalT, TestOutputHandler> pipeline(config, beam_config);

        std::size_t loop_count=4;
        for (std::size_t ii=0; ii<loop_count; ++ii)
        {
            auto tf = std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(number_of_samples, number_of_channels);
            tf->set_channel_frequencies_const_width( f_high, delta );
            tf->sample_interval(t_samp);
            tf->start_time(epoch);
            epoch += std::chrono::duration<double>(tf->sample_interval().value()*number_of_samples);
            noise.next(*tf);
            pipeline(*tf);
        }
        pipeline.output_handler().wait(loop_count);
    }
};

TEST_F(RfiDetectionPipelineTest, uint8_t_run_test)
{
    RfiTests<uint8_t>::run();
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
