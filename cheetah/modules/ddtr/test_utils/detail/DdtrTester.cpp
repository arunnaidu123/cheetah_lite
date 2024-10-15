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
#include "cheetah/modules/ddtr/test_utils/DdtrTester.h"
#include "cheetah/modules/ddtr/Ddtr.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/PulsarInjection.h"
#include "cheetah/generators/PulsarInjectionConfig.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/sigproc/SigProcWriter.h"
#include "unistd.h"
#include <thread>
#include <chrono>
#include <sstream>
#include <cmath>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


template <typename TestTraits>
DdtrTester<TestTraits>::DdtrTester()
    : cheetah::utils::test::AlgorithmTester<TestTraits>()
{
}

template <typename TestTraits>
DdtrTester<TestTraits>::~DdtrTester()
{
}

template<typename TestTraits>
void DdtrTester<TestTraits>::SetUp()
{
}

template<typename TestTraits>
void DdtrTester<TestTraits>::TearDown()
{
}

template<template<typename> class DdtrAlgoT, typename NumericalT>
DdtrTesterTraits<DdtrAlgoT, NumericalT>::DdtrTesterTraits()
    : _dm_call_count(0u)
    , _handler(_dm_data, _dm_call_count)
{
}

template<template<typename> class DdtrAlgoT, typename NumericalT>
typename DdtrTesterTraits<DdtrAlgoT, NumericalT>::Api& DdtrTesterTraits<DdtrAlgoT, NumericalT>::api(PoolType& pool)
{
    typedef ddtr::DedispersionConfig::Dm Dm;
    if(!_api) {
        _config.template config<typename DdtrAlgo::Config>().active(true);
        configure(_config); // call configuration method
        _config.set_pool(pool);
        _beam_config.set_pool(pool);
        _api.reset(new Api(_beam_config, _config, _handler));
    }
    return *_api;
}

template<template<typename> class DdtrAlgoT, typename NumericalT>
typename DdtrTesterTraits<DdtrAlgoT, NumericalT>::DmDataContainerType const&
        DdtrTesterTraits<DdtrAlgoT, NumericalT>::dm_data() const
{
    return _dm_data;
}

template<template<typename> class DdtrAlgoT, typename NumericalT>
ddtr::Config& DdtrTesterTraits<DdtrAlgoT, NumericalT>::config()
{
    return _config;
}

template<template<typename> class DdtrAlgoT, typename NumericalT>
std::size_t DdtrTesterTraits<DdtrAlgoT, NumericalT>::dm_handler_call_count() const
{
    return _dm_call_count;
}

/**
* @brief Generate dispersed pulse with noise in a chunk
*
* @tparam DataType : datatype of the chunk TF ot FT
* @param data : shared pointer to the data
* @param dm : DM of the burts to be intecjed in the data
*/
template<typename DataType>
void generate_dispersed_pulse(std::shared_ptr<DataType> data, float dm)
{
    // generate Pulsar and pass to Ddtr
    // until we get a callback
    generators::GaussianNoiseConfig generator_config;
    generator_config.mean(100);
    generator_config.std_deviation(10);

    generators::GaussianNoise<data::TimeFrequency<Cpu, typename DataType::NumericalRep>> generator(generator_config);

    //instantiate data with noise
    generator.next(*data);
    // Calculate delays and accordingly add 3 bin wide pulse at those positions
    float dm_constant = 4.1493775933609e3;
    std::vector<data::FrequencyType> const& channel_freqs = data->channel_frequencies();
    data::FrequencyType freq_top = *std::max_element(channel_freqs.begin(), channel_freqs.end());
    std::vector<float> delays;
    for (auto freq: channel_freqs)
    {
        delays.push_back(
                          dm_constant
                        * dm
                        * (1.0/(freq.value()*freq.value()) -  1.0/(freq_top.value()*freq_top.value()))
                        / data->sample_interval().value()
                        );
    }

    for(std::size_t ii=0; ii<data->number_of_channels(); ++ii)
    {
        typename DataType::Channel ts = data->channel(ii);
        std::size_t delay = std::size_t((int)(delays[ii]));
        ts[pss::astrotypes::DimensionIndex<pss::astrotypes::units::Time>(delay)] += 100;  //Approx 4 sigma since sigma=24 ;
    }
}

/**
* @brief Finds the distance of the peak in the DM trails object
* @details Essentially returns the dm index which has the peak
* @tparam TraitsParam : TypeParam in the tests
*/
template<typename TraitsParam>
std::size_t find_peak_distance(TraitsParam const& traits)
{
    auto const& dm_data = traits.dm_data();
    auto const& dm_trials = (*dm_data[0]);
    std::vector<float> max_values;
    std::vector<float> trial_nos;
    std::size_t cnt=0;

    for (auto it=dm_trials.cbegin(); it!=dm_trials.cend(); ++it)
    {
        float max_value = *std::max_element(it->cbegin(),it->cend());
        trial_nos.push_back(cnt);
        max_values.push_back(max_value);
        ++cnt;
    }
    return std::distance(max_values.begin(), std::max_element(max_values.begin(),max_values.end()));
}

/**
* @brief Test if the API throws when no range is provided
* @details Throws when range is not specified.
*/
POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_no_ranges_specified)
{
    TypeParam traits;
    ASSERT_THROW(traits.api(pool), panda::Error);
}

/**
* @brief Test if the dedispersion handler is called.
*
* @details Once the agg buffer is filled with enough data, the Dm handler should be triggered and should return a true value
*/

POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_handlers)
{

    //std::cout<<"generating pulse \n";
    TypeParam traits;
    typedef typename TypeParam::NumericalRep NumericalRep;
    typedef typename TypeParam::Dm Dm;
    traits.config().add_dm_range(Dm(0.0 * data::parsecs_per_cube_cm)
                                , Dm(500.0 * data::parsecs_per_cube_cm)
                                , Dm(10.0 * data::parsecs_per_cube_cm));
    traits.config().dedispersion_samples(1<<14);

    auto& api = traits.api(pool);

    typedef typename TypeParam::TimeFrequencyType DataType;

    // keep sending data until we get a callback
    std::size_t count=0;
    std::size_t chunk_samples=10240;
    for(int ii=0; ii < 4; ++ii)
    {
        std::cout<< " passing pulse \n";
        SCOPED_TRACE("In loop on TF block count " + std::to_string(count));
        std::shared_ptr<DataType> data = DataType::make_shared
                        (pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(chunk_samples)
                        , pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(16)
                        );
        data->sample_interval(typename DataType::TimeType(150.0 * data::milliseconds));
        data->set_channel_frequencies_const_width
                        (data::FrequencyType(1000.0 * data::megahertz)
                        , data::FrequencyType(-30.0 * data::megahertz)
                        );

        generate_dispersed_pulse(data, 0.0);
        api(*data);
        ++count;
    }

    ASSERT_TRUE(traits.handler().wait(std::chrono::seconds(5)));
}

/**
* @brief Test if the Dedispersion algorithm works on pulsed data without noise
*
* @details Initialised time frequency data is dispersed at DM=500 with a 1 bin width pulse across x frequency channels. Where X
*         is a randomly generated value between (9, 256). The range of DM trials is 100 to 500 with steps of 10. The Trial
*         number 5 should yield the maximum value of dedispersed pulse.
*/
POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_algorithm)
{
    TypeParam traits;
    typedef typename TypeParam::Dm Dm;
    traits.config().add_dm_range(Dm(0.0 * data::parsecs_per_cube_cm)
                                , Dm(100.0 * data::parsecs_per_cube_cm)
                                , Dm(10.0 * data::parsecs_per_cube_cm));

    traits.config().dedispersion_samples(1<<14);
    auto& api = traits.api(pool);

    // generate Pulsar and pass to Ddtr
    // until we get a callback


    typedef typename TypeParam::TimeFrequencyType DataType;
    // keep sending data until we get a callback

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> nchans(8, 10);

    std::size_t count=0;
    std::size_t chunk_samples = 8192;
    std::size_t no_of_chans = std::pow(2,nchans(mt));
    for(int iter=0; iter<4; iter++)
    {
        std::shared_ptr<DataType> data = DataType::make_shared
                    (pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(chunk_samples)
                    , pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(no_of_chans)
                    );
        data->sample_interval(typename DataType::TimeType(1.0 * data::milliseconds));
        data->set_channel_frequencies_const_width
                    (data::FrequencyType(1000.0 * data::megahertz)
                    , data::FrequencyType(-300.0/(float)no_of_chans * data::megahertz)
                    );

        if(traits.expected_to_pass(*data))
        {
            for (int ii=0; ii<4; ++ii)
            {
                SCOPED_TRACE("In loop on TF block count " + std::to_string(count));
                //instantiate data with zeros
                std::fill(data->begin(), data->end(), 0);

                // Calculate delays and accordingly add 3 bin wide pulse at those positions
                float dm_constant = 4.1493775933609e3;
                std::vector<data::FrequencyType> const& channel_freqs = data->channel_frequencies();
                data::FrequencyType freq_top = *std::max_element(channel_freqs.begin(), channel_freqs.end());
                std::vector<float> delays;
                for (auto freq: channel_freqs)
                {
                    delays.push_back(
                                        dm_constant
                                        * 50.0
                                        * (1.0/(freq.value()*freq.value()) -  1.0/(freq_top.value()*freq_top.value()))
                                        / data->sample_interval().value()
                                    );
                }

                for(std::size_t ii=0; ii<no_of_chans; ++ii)
                {
                    typename DataType::Channel ts = data->channel(ii);
                    std::size_t delay = std::size_t((int)(delays[ii]));
                    ts[pss::astrotypes::DimensionIndex<pss::astrotypes::units::Time>(delay)] = uint8_t(1);
                }

                api(*data);
                ++count;
            }

            ASSERT_TRUE(traits.handler().wait(std::chrono::seconds(15)));

            sync();
            pool.wait();
            ASSERT_EQ(find_peak_distance(traits), 5);
        } else {
            PANDA_LOG << "not running test with nchans = "<< data->number_of_channels() <<" spectra=" << data->number_of_spectra();
        }
    }
}


/**
* @brief Test if the Dedispersion algorithm works on pulsed data with noise
*
* @details Initialised time frequency data is dispersed at DM=500 with a 1 bin width pulse across x frequency channels. Where X
*         is a randomly generated value between (9, 256). The range of DM trials is 100 to 500 with steps of 10. The Trial
*         number 5 should yield the maximum value of dedispersed pulse.
*/
POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_pulse_with_noise)
{
    TypeParam traits;
    typedef typename TypeParam::NumericalRep NumericalRep;
    typedef typename TypeParam::Dm Dm;
    traits.config().add_dm_range(Dm(0.0 * data::parsecs_per_cube_cm)
                                , Dm(50.0 * data::parsecs_per_cube_cm)
                                , Dm(1.0 * data::parsecs_per_cube_cm));
    traits.config().dedispersion_samples(1<<14);

    auto& api = traits.api(pool);
    typedef typename TypeParam::TimeFrequencyType DataType;

    // keep sending data until we get a callback

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> nspectra(8000, 10000);
    std::uniform_int_distribution<std::size_t> nchans(7, 9);
    std::size_t no_of_chans= std::pow(2, nchans(mt));

    for(int iter=0; iter<4; iter++)
    {
        std::size_t chunk_samples= nspectra(mt);

        std::shared_ptr<DataType> data = DataType::make_shared
                            (pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(chunk_samples)
                            , pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(no_of_chans)
                            );
        data->sample_interval(typename DataType::TimeType(1.0 * data::milliseconds));
        data->set_channel_frequencies_const_width
                            (data::FrequencyType(1000.0 * data::megahertz)
                            , data::FrequencyType(-1*std::floor( 300.0/((float)no_of_chans) * 100)/100.0 * data::megahertz)
                            );

        if(traits.expected_to_pass(*data))
        {
            for (int ii=0; ii<4; ++ii)
            {
                generate_dispersed_pulse(data, 5.0);
                api(*data);
            }

            ASSERT_TRUE(traits.handler().wait(std::chrono::seconds(5)));

            sync();
            pool.wait();

            ASSERT_EQ(find_peak_distance(traits), 5);
        }
    }
}

/**
* @brief Test if the Dedispersion algorithm works on pulsed data with noise
*
* @details Initialised time frequency data is dispersed at DM=500 with a 1 bin width pulse across x frequency channels. Where X
*         is a randomly generated value between (9, 256). The range of DM trials is 100 to 500 with steps of 10. The Trial
*         number 5 should yield the maximum value of dedispersed pulse.
*/

POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_pulse_with_noise_dm_range)
{
    TypeParam traits;
    typedef typename TypeParam::NumericalRep NumericalRep;
    typedef typename TypeParam::Dm Dm;
    traits.config().add_dm_range(Dm(0.0 * data::parsecs_per_cube_cm)
                                , Dm(100.0 * data::parsecs_per_cube_cm)
                                , Dm(10.0 * data::parsecs_per_cube_cm));

    traits.config().add_dm_range(Dm(100.0 * data::parsecs_per_cube_cm)
                                , Dm(300.0 * data::parsecs_per_cube_cm)
                                , Dm(20.0 * data::parsecs_per_cube_cm));
    traits.config().dedispersion_samples(1<<14);

    auto& api = traits.api(pool);
    typedef typename TypeParam::TimeFrequencyType DataType;

    // keep sending data until we get a callback

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> nspectra(8000, 10000);
    std::uniform_int_distribution<std::size_t> nchans(3, 7);
    std::size_t no_of_chans= std::pow(2, nchans(mt));

    for(int iter=0; iter<4; iter++)
    {
        std::size_t chunk_samples= nspectra(mt);

        std::shared_ptr<DataType> data = DataType::make_shared
                            (pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(chunk_samples)
                            , pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(no_of_chans)
                            );
        data->sample_interval(typename DataType::TimeType(5.0 * data::milliseconds));
        data->set_channel_frequencies_const_width
                            (data::FrequencyType(1000.0 * data::megahertz)
                            , data::FrequencyType(-1*std::floor( 300.0/((float)no_of_chans) * 100)/100.0 * data::megahertz)
                            );

        if(traits.expected_to_pass(*data))
        {
            for (int ii=0; ii<4; ++ii)
            {
                generate_dispersed_pulse(data, 200.0);
                api(*data);
            }

            ASSERT_TRUE(traits.handler().wait(std::chrono::seconds(5)));

            sync();
            pool.wait();

            ASSERT_EQ(find_peak_distance(traits), 15);
        }
    }
}

/**
* @brief Test if the Dedispersion algorithm specifically for SKA LOW
*
* @details Initialised time frequency data is dispersed at DM=50 with a 1 bin width pulse across 7776 frequency channels.
*          The range of DM trials is 0 to 50 with steps of 1. The Trial
*         number 10 should yield the maximum value of dedispersed pulse.
*/

/**
* @test generic tester for SKA-LOW senario
* @given a ddtr module
* @when run through the test_ska_low_senario
* @then generates random TF data with a dispersed pulse of 5 pc/cm3 then transforms the TF object to generates dmtrails
*       object which should have a peak at the 5th dmtrail.
*/
POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_ska_low_senario)
{
    TypeParam traits;
    typedef typename TypeParam::NumericalRep NumericalRep;
    typedef typename TypeParam::Dm Dm;
    traits.config().add_dm_range(Dm(0.0 * data::parsecs_per_cube_cm)
                                , Dm(10.0 * data::parsecs_per_cube_cm)
                                , Dm(1.0 * data::parsecs_per_cube_cm));
    traits.config().dedispersion_samples(1<<14);

    auto& api = traits.api(pool);

    // generate Pulsar and pass to Ddtr
    // until we get a callback
    generators::GaussianNoiseConfig generator_config;
    generator_config.mean(100);
    generator_config.std_deviation(10);

    generators::GaussianNoise<data::TimeFrequency<Cpu, NumericalRep>> generator(generator_config);

    typedef typename TypeParam::TimeFrequencyType DataType;

    // keep sending data until we get a callback

    std::size_t no_of_chans= 7776;
    std::size_t chunk_samples= 8192;

    std::shared_ptr<DataType> data = DataType::make_shared
                            (pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(chunk_samples)
                            , pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(no_of_chans)
                            );
    data->sample_interval(typename DataType::TimeType(0.1 * data::milliseconds));
    data->set_channel_frequencies_const_width
                        (data::FrequencyType(350.0 * data::megahertz)
                        , data::FrequencyType(-1*std::floor( 100.0/((float)no_of_chans) * 100)/100.0 * data::megahertz)
                        );
    if(traits.expected_to_pass(*data))
    {
        for (int ii=0; ii<4; ++ii)
        {
            //instantiate data with noise
            generate_dispersed_pulse(data, 5.0);
            api(*data);
        }

        ASSERT_TRUE(traits.handler().wait(std::chrono::seconds(50)));

        sync();
        pool.wait();

        ASSERT_EQ(find_peak_distance(traits), 5);
    }
}

/**
* @brief Test if the Dedispersion algorithm specifically for SKA MID
*
* @details Initialised time frequency data is dispersed at DM=50 with a 1 bin width pulse across 3700 frequency channels.
*          The range of DM trials is 0 to 100 with steps of 10. The Trial
*         number 5 should yield the maximum value of dedispersed pulse.
*/

/**
* @test generic tester for SKA-MID senario
* @given a ddtr module
* @when run through the test_ska_mid_senario
* @then generates random TF data with a dispersed pulse of 50 pc/cm3 and transforms the TF object using the ddtr module to
*       generates dmtrails object which should have a peak at the 5th dmtrail.
*/

POOL_ALGORITHM_TYPED_TEST_P(DdtrTester, test_ska_mid_senario)
{
    TypeParam traits;
    typedef typename TypeParam::NumericalRep NumericalRep;
    typedef typename TypeParam::Dm Dm;
    traits.config().add_dm_range(Dm(0.0 * data::parsecs_per_cube_cm)
                                , Dm(100.0 * data::parsecs_per_cube_cm)
                                , Dm(10.0 * data::parsecs_per_cube_cm));
    traits.config().dedispersion_samples(1<<14);

    auto& api = traits.api(pool);

    typedef typename TypeParam::TimeFrequencyType DataType;

    // keep sending data until we get a callback

    std::size_t no_of_chans= 3700;
    std::size_t chunk_samples= 8192;

    std::shared_ptr<DataType> data = DataType::make_shared
                            (pss::astrotypes::DimensionSize<pss::astrotypes::units::Time>(chunk_samples)
                            , pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency>(no_of_chans)
                            );
    data->sample_interval(typename DataType::TimeType(0.05 * data::milliseconds));
    data->set_channel_frequencies_const_width
                        (data::FrequencyType(1400.0 * data::megahertz)
                        , data::FrequencyType(-1*std::floor( 300.0/((float)no_of_chans) * 100)/100.0 * data::megahertz)
                        );

    if(traits.expected_to_pass(*data))
    {
        for (int ii=0; ii<4; ++ii)
        {
            //instantiate data with noise
            generate_dispersed_pulse(data, 50);
            api(*data);
        }

        ASSERT_TRUE(traits.handler().wait(std::chrono::seconds(50)));

        sync();
        pool.wait();

        ASSERT_EQ(find_peak_distance(traits), 5);
    }
}

// each test defined by ALGORITHM_TYPED_TEST_P must be added to the
// test register (each one as an element of the comma seperated list)
REGISTER_TYPED_TEST_SUITE_P(DdtrTester
                          , test_handlers
                          , test_no_ranges_specified
                          , test_algorithm
                          , test_pulse_with_noise
                          , test_pulse_with_noise_dm_range
                          , test_ska_low_senario
                          , test_ska_mid_senario
                          );
} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
