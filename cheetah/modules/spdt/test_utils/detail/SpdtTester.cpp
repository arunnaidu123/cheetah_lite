/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#include "cheetah/modules/spdt/test_utils/SpdtTester.h"
#include "cheetah/modules/spdt/Spdt.h"
#include "cheetah/data/RfimFlaggedData.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/DispersedPulse.h"
#include "cheetah/generators/RfiScenario.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace test {


template<int ScenarioNum, typename TimeFrequencyType>
class TestRfiData: public data::RfimFlaggedData<TimeFrequencyType>
{
    private:
        typedef data::RfimFlaggedData<TimeFrequencyType> BaseT;

    public:
        TestRfiData(TimeFrequencyType& data);
        ~TestRfiData();

    private:
        generators::RfiScenario<ScenarioNum, typename TimeFrequencyType::NumericalRep> _generator;

};

template<int ScenarioNum, typename TimeFrequencyType>
TestRfiData<ScenarioNum, TimeFrequencyType>::TestRfiData(TimeFrequencyType& data)
    : BaseT(data)
{
    _generator.next(*this);
}

template<int ScenarioNum, typename TimeFrequencyType>
TestRfiData<ScenarioNum, TimeFrequencyType>::~TestRfiData()
{
}


template<typename RfiDataType, typename TraitsType, typename PoolType>
void test_single_pulse_with_noise(TraitsType& traits, PoolType& pool)
{
    auto& spdt_config = traits.config();
    modules::ddtr::Config& ddtr_config = spdt_config.ddtr_config();
    typedef typename modules::ddtr::DedispersionConfig::Dm Dm;

    ddtr_config.add_dm_range(Dm(0.0 * data::parsec_per_cube_cm)
                            ,Dm(50.0 * data::parsec_per_cube_cm)
                            ,Dm(1.0 * data::parsec_per_cube_cm));
    ddtr_config.dedispersion_samples(1<<14);

    auto& api = traits.api(pool);



    typedef typename data::SpCcl<unsigned char>::SpCandidateType CandidateType;
    typedef typename CandidateType::MsecTimeType TimeType;
    typedef spdt::Spdt<spdt::Config, uint8_t> SpdtType;
    typedef typename SpdtType::TimeFrequencyType DataType;
    pss::astrotypes::DimensionSize<pss::astrotypes::units::Time> chunk_samples(8192);
    pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency> number_of_channels(64);
    std::shared_ptr<DataType> data = DataType::make_shared(chunk_samples, number_of_channels);

    // generate random noise data and pass to Spdt
    // until we get a callback
    generators::GaussianNoiseConfig generator_config;
    generators::GaussianNoise<DataType> generator(generator_config);
    typedef spdt::Spdt<spdt::Config, uint8_t> SpdtType;


    std::size_t count=0;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> rand_dm(10, 40);
    float dm = rand_dm(mt);
    TimeType data_end_time(0);
    while(!traits.dm_handler_called())
    {
        auto data = DataType::make_shared(chunk_samples, number_of_channels);
        data->sample_interval(boost::units::quantity<pss::astrotypes::units::Seconds, double>(0.5 * data::milliseconds));
        data->set_channel_frequencies_const_width(data::FrequencyType(1000.0 * data::megahertz), data::FrequencyType(-300/64.0 * data::megahertz));
        std::fill(data->begin(), data->end(), 0);

        generator.next(*data);

        // Calculate delays and accordingly add 3 bin wide pulse at those positions
        float dm_constant = 4.1493775933609e3;
        std::vector<data::FrequencyType> const& channel_freqs = data->channel_frequencies();
        data::FrequencyType freq_top = *std::max_element(channel_freqs.begin(), channel_freqs.end());
        std::vector<float> delays;
        for (auto freq: channel_freqs)
        {
            delays.push_back(dm_constant * dm * (1.0/(freq.value()*freq.value()) -  1.0/(freq_top.value()*freq_top.value())) / data->sample_interval().value());
        }

        for(std::size_t ii=0; ii<number_of_channels; ++ii)
        {
            typename DataType::Channel ts = data->channel(ii);
            std::size_t delay = std::size_t((int)(delays[ii]));
            for(unsigned int i=0; i<32; i++) ts[pss::astrotypes::DimensionIndex<pss::astrotypes::units::Time>(delay+i)] += 10;
        }

        api(*data);
        data_end_time += static_cast<std::size_t>(chunk_samples) * TimeType(data->sample_interval());
        ++count;
    }

    ASSERT_TRUE(traits.wait_sp_handler_called());
    auto sp_data_ptr = traits.sp_data();
    auto const& sp_data = *sp_data_ptr;

    ASSERT_GE(sp_data.size(), 1U);

    // verify a candidate is found with the test dm
    auto check_for_candidate = [dm](CandidateType const& candidate){ return candidate.dm().value() == dm; };
    ASSERT_TRUE(std::any_of(sp_data.begin(), sp_data.end(), check_for_candidate));

    // ensure start time of candidates is within expected bounds
    for( auto const& candidate : sp_data) {
        ASSERT_TRUE(candidate.tstart() < data_end_time);
        ASSERT_TRUE(candidate.tend() > TimeType(0));
    }
}


/*
template<typename RfiDataType, typename TraitsType, typename PoolType>
void test_single_pulse_with_noise_ska_low(TraitsType& traits, PoolType& pool)
{
    auto& spdt_config = traits.config();
    modules::ddtr::Config& ddtr_config = spdt_config.ddtr_config();
    typedef typename modules::ddtr::DedispersionConfig::Dm Dm;

    ddtr_config.add_dm_range(Dm(0.0 * data::parsec_per_cube_cm)
                            ,Dm(10.0 * data::parsec_per_cube_cm)
                            ,Dm(1.0 * data::parsec_per_cube_cm));
    ddtr_config.dedispersion_samples(1<<15);

    auto& api = traits.api(pool);



    typedef typename data::SpCcl<unsigned char>::SpCandidateType CandidateType;
    typedef typename CandidateType::MsecTimeType TimeType;
    typedef spdt::Spdt<spdt::Config, uint8_t> SpdtType;
    typedef typename SpdtType::TimeFrequencyType DataType;
    pss::astrotypes::DimensionSize<pss::astrotypes::units::Time> chunk_samples(8192);
    pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency> number_of_channels(7776);
    std::shared_ptr<DataType> data = DataType::make_shared(chunk_samples, number_of_channels);

    // generate random noise data and pass to Spdt
    // until we get a callback
    generators::GaussianNoiseConfig generator_config;
    generators::GaussianNoise<DataType> generator(generator_config);


    std::size_t count=0;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> rand_dm(0, 9);
    float dm = rand_dm(mt);
    while(!traits.dm_handler_called())
    {
        auto data = DataType::make_shared(chunk_samples, number_of_channels);
        data->sample_interval(boost::units::quantity<pss::astrotypes::units::Seconds, double>(0.1 * data::milliseconds));
        data->set_channel_frequencies_const_width(data::FrequencyType(350.0 * data::megahertz), data::FrequencyType(-100/7776.0 * data::megahertz));
        std::fill(data->begin(), data->end(), 0);

        generator.next(*data);

        // Calculate delays and accordingly add 3 bin wide pulse at those positions
        float dm_constant = 4.1493775933609e3;
        std::vector<data::FrequencyType> const& channel_freqs = data->channel_frequencies();
        data::FrequencyType freq_top = *std::max_element(channel_freqs.begin(), channel_freqs.end());
        std::vector<float> delays;
        for (auto freq: channel_freqs)
        {
            delays.push_back(dm_constant * dm * (1.0/(freq.value()*freq.value()) -  1.0/(freq_top.value()*freq_top.value())) / data->sample_interval().value());
        }

        for(std::size_t ii=0; ii<number_of_channels; ++ii)
        {
            typename DataType::Channel ts = data->channel(ii);
            std::size_t delay = std::size_t((int)(delays[ii]));
            for(unsigned int i=0; i<32; i++) ts[pss::astrotypes::DimensionIndex<pss::astrotypes::units::Time>(delay+i)] += 10;  //Approx 4 sigma since sigma=24 ;
        }

        api(*data);
        ++count;
    }

    ASSERT_TRUE(traits.wait_sp_handler_called());
    auto sp_data_ptr = traits.sp_data();
    auto const& sp_data = *sp_data_ptr;

    ASSERT_GE(sp_data.size(), 1U);

    // verify a candidate is found with the test dm
    auto check_for_candidate = [dm](CandidateType const& candidate){ return candidate.dm().value() == dm; };
    ASSERT_TRUE(std::any_of(sp_data.begin(), sp_data.end(), check_for_candidate));

}


template<typename RfiDataType, typename TraitsType, typename PoolType>
void test_single_pulse_with_noise_ska_mid(TraitsType& traits, PoolType& pool)
{
    auto& spdt_config = traits.config();
    modules::ddtr::Config& ddtr_config = spdt_config.ddtr_config();
    typedef typename modules::ddtr::DedispersionConfig::Dm Dm;

    ddtr_config.add_dm_range(Dm(0.0 * data::parsec_per_cube_cm)
                            ,Dm(10.0 * data::parsec_per_cube_cm)
                            ,Dm(1.0 * data::parsec_per_cube_cm));
    ddtr_config.dedispersion_samples(1<<15);

    auto& api = traits.api(pool);



    typedef typename data::SpCcl<unsigned char>::SpCandidateType CandidateType;
    typedef typename CandidateType::MsecTimeType TimeType;
    typedef spdt::Spdt<spdt::Config, uint8_t> SpdtType;
    typedef typename SpdtType::TimeFrequencyType DataType;
    pss::astrotypes::DimensionSize<pss::astrotypes::units::Time> chunk_samples(8192);
    pss::astrotypes::DimensionSize<pss::astrotypes::units::Frequency> number_of_channels(3700);
    std::shared_ptr<DataType> data = DataType::make_shared(chunk_samples, number_of_channels);

    // generate random noise data and pass to Spdt
    // until we get a callback
    generators::GaussianNoiseConfig generator_config;
    generators::GaussianNoise<DataType> generator(generator_config);


    std::size_t count=0;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> rand_dm(0, 9);
    float dm = rand_dm(mt);
    while(!traits.dm_handler_called())
    {
        auto data = DataType::make_shared(chunk_samples, number_of_channels);
        data->sample_interval(boost::units::quantity<pss::astrotypes::units::Seconds, double>(0.05 * data::milliseconds));
        data->set_channel_frequencies_const_width(data::FrequencyType(1550.0 * data::megahertz), data::FrequencyType(-300/3700.0 * data::megahertz));
        std::fill(data->begin(), data->end(), 0);

        generator.next(*data);

        // Calculate delays and accordingly add 3 bin wide pulse at those positions
        float dm_constant = 4.1493775933609e3;
        std::vector<data::FrequencyType> const& channel_freqs = data->channel_frequencies();
        data::FrequencyType freq_top = *std::max_element(channel_freqs.begin(), channel_freqs.end());
        std::vector<float> delays;
        for (auto freq: channel_freqs)
        {
            delays.push_back(dm_constant * dm * (1.0/(freq.value()*freq.value()) -  1.0/(freq_top.value()*freq_top.value())) / data->sample_interval().value());
        }

        for(std::size_t ii=0; ii<number_of_channels; ++ii)
        {
            typename DataType::Channel ts = data->channel(ii);
            std::size_t delay = std::size_t((int)(delays[ii]));
            for(unsigned int i=0; i<32; i++) ts[pss::astrotypes::DimensionIndex<pss::astrotypes::units::Time>(delay+i)] += 10;  //Approx 4 sigma since sigma=24 ;
        }

        api(*data);
        ++count;
    }

    ASSERT_TRUE(traits.wait_sp_handler_called());
    auto sp_data_ptr = traits.sp_data();
    auto const& sp_data = *sp_data_ptr;

    ASSERT_GE(sp_data.size(), 1U);

    // verify a candidate is found with the test dm
    auto check_for_candidate = [dm](CandidateType const& candidate){ return candidate.dm().value() == dm; };
    ASSERT_TRUE(std::any_of(sp_data.begin(), sp_data.end(), check_for_candidate));

}
*/

template <typename TestTraits>
SpdtTester<TestTraits>::SpdtTester()
    : cheetah::utils::test::AlgorithmTester<TestTraits>()
{
}

template <typename TestTraits>
SpdtTester<TestTraits>::~SpdtTester()
{
}

template<typename TestTraits>
void SpdtTester<TestTraits>::SetUp()
{
}

template<typename TestTraits>
void SpdtTester<TestTraits>::TearDown()
{
}


template<typename ArchitectureTag, typename ArchitectureCapability>
SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::SpdtTesterTraits()
    : _dm_called(false)
    , _sp_called(false)
    , _sp_call_count(0u)
{
}

template<typename ArchitectureTag, typename ArchitectureCapability>
typename SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::Api&
        SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::api(PoolType& pool)
{
    if(!_api) {
        configure(_config); // call configuration method
        _config.set_pool(pool);
        _config.ddtr_config().dedispersion_samples(1<<16);
        _api.reset(new Api(_config,
                     [this](std::shared_ptr<DmType> data)
                     {
                        _dm_called = true;
                        _dm_data = data;
                     },
                     [this](std::shared_ptr<SpType> data)
                     {
                        try {
                            std::lock_guard<std::mutex> lock(_sp_data_mutex);
                            {
                                _sp_called = true;
                                ++_sp_call_count;
                                _sp_data.push_back(data);
                            }
                        } catch(...) {}
                        _sp_wait.notify_all();
                     }));
    }
    return *_api;
}

template<typename ArchitectureTag, typename ArchitectureCapability>
spdt::Config& SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::config()
{
    return _config;
}

template<typename ArchitectureTag, typename ArchitectureCapability>
bool SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::dm_handler_called() const
{
    return _dm_called;
}

template<typename ArchitectureTag, typename ArchitectureCapability>
bool SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::sp_handler_called() const
{
    return _sp_called;
}

template<typename ArchitectureTag, typename ArchitectureCapability>
bool SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::wait_sp_handler_called() const
{
    std::unique_lock<std::mutex> lock(_sp_data_mutex);
    _sp_wait.wait_for(lock, std::chrono::seconds(5), [this] { return _sp_called; });
    return _sp_called;
}

template<typename ArchitectureTag, typename ArchitectureCapability>
std::shared_ptr<typename SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::SpType> SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::sp_data() const
{
    std::lock_guard<std::mutex> lock(_sp_data_mutex);
    auto data_ptr = _sp_data.front();
    _sp_data.pop_front();
    return data_ptr;
}

template<typename ArchitectureTag, typename ArchitectureCapability>
std::size_t SpdtTesterTraits<ArchitectureTag, ArchitectureCapability>::sp_handler_call_count() const
{
    return _sp_call_count;
}


POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, test_handlers)
{
    TypeParam traits;
    auto& spdt_config = traits.config();
    modules::ddtr::Config& ddtr_config = spdt_config.ddtr_config();
    typedef typename modules::ddtr::DedispersionConfig::Dm Dm;

    ddtr_config.add_dm_range(Dm(0.0 * data::parsec_per_cube_cm)
                            ,Dm(500.0 * data::parsec_per_cube_cm)
                            ,Dm(10.0 * data::parsec_per_cube_cm));
    ddtr_config.dedispersion_samples(1<<14);
    auto& api = traits.api(pool);


    // generate random noise data and pass to Spdt
    // until we get a callback
    typedef spdt::Spdt<spdt::Config, uint8_t> SpdtType;
    typedef typename SpdtType::TimeFrequencyType DataType;
    generators::GaussianNoiseConfig generator_config;
    generators::GaussianNoise<DataType> generator(generator_config);


    // keep sending data until we get a callback
    std::size_t count=0;
    std::size_t chunk_samples=1024;
    while(!traits.sp_handler_called())
    {
        auto data = DataType::make_shared(data::DimensionSize<data::Time>(chunk_samples), data::DimensionSize<data::Frequency>(64));
        data->sample_interval(DataType::TimeType(1.0 * data::milliseconds));
        data->set_channel_frequencies_const_width(data::FrequencyType(1000.0 * data::megahertz), data::FrequencyType(-0.05 * data::megahertz));
        generator.next(*data);
        api(*data);
        ++count;
    }

    ASSERT_TRUE(traits.dm_handler_called());
    // now we expect the buffer to flush data as it is destroyed. so the pool has to stay alive whilst this happens

}
/*
POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_1)
{
    TypeParam traits;
    test_single_pulse_with_noise<TestRfiData<1, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}

POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_2)
{
    TypeParam traits;
    test_single_pulse_with_noise<TestRfiData<2, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}

POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_3)
{
    TypeParam traits;
    test_single_pulse_with_noise<TestRfiData<3, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}

POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_4)
{
    TypeParam traits;
    test_single_pulse_with_noise<TestRfiData<4, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}

POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_5)
{
    TypeParam traits;
    test_single_pulse_with_noise<TestRfiData<5, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}


POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_ska_low)
{
    TypeParam traits;
    test_single_pulse_with_noise_ska_low<TestRfiData<6, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}

POOL_ALGORITHM_TYPED_TEST_P(SpdtTester, single_pulse_with_noise_ska_mid)
{
    TypeParam traits;
    test_single_pulse_with_noise_ska_mid<TestRfiData<7, data::TimeFrequency<Cpu, uint8_t>>>(traits, pool);
}

// each test defined by ALGORITHM_TYPED_TEST_P must be added to the
// test register (each one as an element of the comma seperated list)
REGISTER_TYPED_TEST_SUITE_P(SpdtTester
                        , test_handlers
                        , single_pulse_with_noise_1
                        , single_pulse_with_noise_2
                        , single_pulse_with_noise_3
                        , single_pulse_with_noise_4
                        , single_pulse_with_noise_5
                        , single_pulse_with_noise_ska_low
                        , single_pulse_with_noise_ska_mid);

*/
REGISTER_TYPED_TEST_SUITE_P(SpdtTester, test_handlers);
} // namespace test
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
