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
#include "cheetah/pipelines/search_pipeline/test/SinglePulseTest.h"
#include "cheetah/pipelines/search_pipeline/SinglePulse.h"
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"
#include "cheetah/modules/ddtr/DedispersionConfig.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include <memory>


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


SinglePulseTest::SinglePulseTest()
    : ::testing::Test()
{
}

SinglePulseTest::~SinglePulseTest()
{
}

void SinglePulseTest::SetUp()
{
}

void SinglePulseTest::TearDown()
{
}

template<typename NumericalType>
using TestSinglePulsePipeline = SinglePulse<NumericalType>;

template<typename NumericalT>
struct SinglePulseTests
{
    static void run()
    {
        /**
         * Search ~600 seconds of data at standard resoluton
         */
        typedef typename data::TimeFrequency<Cpu, NumericalT>::TimePointType TimePointType;
        typedef typename data::TimeFrequency<Cpu, NumericalT>::DataType DataType;


        /**
         * Configure components
         */
        CheetahConfig<NumericalT> config;

        //Configure high density low DMs
        modules::ddtr::DedispersionConfig dd_config_low;
        config.ddtr_config().dedispersion_config(dd_config_low);
        config.spdt_config().cpu_config().activate();
        dd_config_low.dm_start(modules::ddtr::DedispersionConfig::Dm(0.0 * data::parsecs_per_cube_cm));
        dd_config_low.dm_end(modules::ddtr::DedispersionConfig::Dm(100.0 * data::parsecs_per_cube_cm));
        dd_config_low.dm_step(modules::ddtr::DedispersionConfig::Dm(0.1 * data::parsecs_per_cube_cm));

        //Configure low density high DMs
        /*
        modules::ddtr::DedispersionConfig dd_config_high;
        config.sps_config().dedispersion_config(dd_config_high);
        dd_config_high.dm_start(modules::ddtr::DedispersionConfig::Dm(200.0 * data::parsecs_per_cube_cm));
        dd_config_high.dm_end(modules::ddtr::DedispersionConfig::Dm(300.0 * data::parsecs_per_cube_cm));
        dd_config_high.dm_step(modules::ddtr::DedispersionConfig::Dm(1.0 * data::parsecs_per_cube_cm));
*/
        auto& cpu_config = config.ddtr_config().cpu_algo_config();
        cpu_config.active(true);

        //Set sps priority
        config.ddtr_config().set_priority(0);

        //Set the size of the dedispersion buffer
        config.ddtr_config().dedispersion_samples(1<<17);

        //Set up noise parameters for data to be passed through
        //the pipeline
        generators::GaussianNoiseConfig noise_config;
        noise_config.mean(96.0);
        noise_config.std_deviation(10.0);
        generators::GaussianNoise<data::TimeFrequency<Cpu, NumericalT>> noise(noise_config);
        BeamConfig<uint8_t> beam_config;

        //Start epoch
        typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
        TestSinglePulsePipeline<NumericalT> pipeline(config, beam_config);

        double tsamp_us = 64.0;
        double f_low = 1.2;
        double f_high = 1.8;
        std::size_t total_nsamps = std::size_t(10.0 / (tsamp_us * 1e-6));
        data::DimensionSize<data::Time> number_of_samples(1<<15);
        data::DimensionSize<data::Frequency> number_of_channels(1024);
        std::size_t loop_count = total_nsamps/number_of_samples + 10;
        for (std::size_t ii=0; ii<loop_count; ++ii)
        {
            auto tf = std::make_shared<data::TimeFrequency<Cpu, uint8_t>>(number_of_samples, number_of_channels);
            auto f1 =  data::TimeFrequency<Cpu, uint8_t>::FrequencyType(f_high * boost::units::si::giga * boost::units::si::hertz);
            auto f2 =  data::TimeFrequency<Cpu, uint8_t>::FrequencyType(f_low * boost::units::si::giga * boost::units::si::hertz);
            auto delta = (f2 - f1)/ (double)number_of_channels;
            tf->set_channel_frequencies_const_width( f1, delta );
            tf->sample_interval(typename data::TimeFrequency<Cpu, uint8_t>::TimeType(tsamp_us * boost::units::si::micro * data::seconds));
            tf->start_time(epoch);
            epoch += std::chrono::duration<double>(tf->sample_interval().value()*number_of_samples);
            std::cout << "Time frequency block " << ii << " dispatched to pipeline." << std::endl;
            noise.next(*tf);
            pipeline(*tf);
        }
    }
};

TEST_F(SinglePulseTest, uint8_t_run_test)
{
    SinglePulseTests<uint8_t>::run();
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
