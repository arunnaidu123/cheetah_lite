/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2024 The SKA organisation
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
#include "cheetah/pipelines/search_pipeline/test/FdasAccelerationSearchTest.h"
#include "cheetah/pipelines/search_pipeline/AccelerationSearch.h"
#include "cheetah/pipelines/search_pipeline/FdasAccelerationSearchTraits.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/modules/ddtr/DedispersionConfig.h"
#include "cheetah/generators/SimplePulsar.h"
#include "cheetah/generators/SimplePulsarConfig.h"
#include "cheetah/generators/PulsarInjection.h"
#include "cheetah/generators/PulsarInjectionConfig.h"
#include "cheetah/generators/SimplePhaseModelConfig.h"
#include <memory>
#include <chrono>
#include <thread>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {

FdasAccelerationSearchTest::FdasAccelerationSearchTest()
    : ::testing::Test()
{
}

FdasAccelerationSearchTest::~FdasAccelerationSearchTest()
{
}

void FdasAccelerationSearchTest::SetUp()
{
}

void FdasAccelerationSearchTest::TearDown()
{
}

template<typename NumericalT>
struct FdasAccelerationSearchTests
{
    static void run()
    {
        // Search ~600 seconds of data at standard resoluton
        float total_time = 560.0;
        typedef data::TimeFrequency<Cpu, NumericalT> TimeFrequencyType;
        typedef typename TimeFrequencyType::DataType DataType;


        // Configure components
        CheetahConfig<NumericalT> config;

        // Configure high-density low DMs
        config.ddtr_config().add_dm_range(modules::ddtr::DedispersionConfig::Dm(0.0 * data::parsecs_per_cube_cm)
                                  , modules::ddtr::DedispersionConfig::Dm(100 * data::parsecs_per_cube_cm)
                                  , modules::ddtr::DedispersionConfig::Dm(0.1 * data::parsecs_per_cube_cm));

        config.ddtr_config().add_dm_range(modules::ddtr::DedispersionConfig::Dm(100.0 * data::parsecs_per_cube_cm)
                                  , modules::ddtr::DedispersionConfig::Dm(300 * data::parsecs_per_cube_cm)
                                  , modules::ddtr::DedispersionConfig::Dm(0.2 * data::parsecs_per_cube_cm));

        config.ddtr_config().dedispersion_samples(1<<18);
        config.ddtr_config().cpu_algo_config().active(true);
        config.sps_config().cpu_config().activate();

        // Set sps priority
        config.sps_config().set_priority(0);

        // Set the size of the dedispersion buffer
        config.sps_config().ddtr_config().dedispersion_samples(1<<18);

        // Set the psbc config
        config.psbc_config().dump_time((total_time-20.0) * data::seconds);

        //config.fft_config().time_real_to_complex().fftw_config().active(true);

        // Set fdas priority
        config.acceleration_search_config().fdas_config().set_priority(1);

        // Set up labyrinth implementation specific configuration arguments
        config.acceleration_search_config().fdas_config().labyrinth_config().active(true);

        // Birdies configuration would go here

        // select the sift algo
        config.sift_config().template config<modules::sift::simple_sift::Config>().active(true);

        // select the fldo algo
        config.fldo_config().template config<modules::fldo::cuda::Config>().active(true);

        BeamConfig<NumericalT> beam_config;
        typedef AccelerationSearch<NumericalT, FdasAccelerationSearchTraits<NumericalT>> AccelerationSearchType;
        AccelerationSearchType pipeline(config, beam_config);

        // Set up pulsar parameters for data to be passed through the pipeline
        generators::SimplePulsarConfig pulsar_config;
        pulsar_config.mean(96.0);
        pulsar_config.std_deviation(10.0);
        pulsar_config.dm(200.0*data::parsecs_per_cube_cm);
        pulsar_config.period(0.005*data::seconds);
        pulsar_config.width(0.001*data::seconds);
        pulsar_config.snr(1.0);
        generators::SimplePulsar<TimeFrequencyType> pulsar(pulsar_config);

        // Start epoch
        typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));

        double tsamp_us = 64.0;
        double f_low = 600.0;
        double f_step = 330.0/1024.0;
        std::size_t total_nsamps = std::size_t(total_time / (tsamp_us * 1e-6));
        data::DimensionSize<data::Time> number_of_samples(1024);
        data::DimensionSize<data::Frequency> number_of_channels(1024);
        std::size_t loop_count = total_nsamps/number_of_samples + 10;
        for (std::size_t ii=0; ii<loop_count; ++ii)
        {
            auto tf = std::make_shared<TimeFrequencyType>(number_of_samples, number_of_channels);
            auto f1 =  typename TimeFrequencyType::FrequencyType((f_low+f_step*number_of_channels) * boost::units::si::mega * boost::units::si::hertz);
            auto f2 =  typename TimeFrequencyType::FrequencyType(f_low * boost::units::si::mega * boost::units::si::hertz);
            auto delta = (f2 - f1)/ (double)number_of_channels;
            tf->set_channel_frequencies_const_width( f1, delta );
            tf->sample_interval(typename TimeFrequencyType::TimeType(tsamp_us * boost::units::si::micro * data::seconds));
            tf->start_time(epoch);
            epoch += std::chrono::duration<double>(tf->sample_interval().value()*number_of_samples);
            pulsar.next(*tf);
            pipeline(*tf);
        }
    }
};

TEST_F(FdasAccelerationSearchTest, run_fdas_test_uint8)
{
    #ifdef NDEBUG
    #ifdef SKA_CHEETAH_ENABLE_NASM
    FdasAccelerationSearchTests<uint8_t>::run();
    #endif
    #endif
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska