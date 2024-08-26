/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 The SKA organisation
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
#include "cheetah/modules/rfim/test_utils/RfimTester.h"
#include "cheetah/sigproc/SigProcWriter.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include "cheetah/generators/DispersedPulse.h"
#include "cheetah/generators/RfiScenario.h"
#include "cheetah/data/TimeFrequency.h"
#include "panda/test/TestDir.h"
#include <boost/units/systems/si/frequency.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <limits>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {


template <typename TestTraits>
RfimTester<TestTraits>::RfimTester()
    : cheetah::utils::test::AlgorithmTester<TestTraits>()
{
}

template <typename TestTraits>
RfimTester<TestTraits>::~RfimTester()
{
}

template<typename TestTraits>
void RfimTester<TestTraits>::SetUp()
{
}

template<typename TestTraits>
void RfimTester<TestTraits>::TearDown()
{
}

template<typename TestTraits>
void RfimTester<TestTraits>::verify_equal(typename TestTraits::DataType const& d1, typename TestTraits::DataType const& d2)
{
    ASSERT_EQ(d1.number_of_samples(), d2.number_of_samples());
    ASSERT_EQ(d1.number_of_channels(), d2.number_of_channels());
    auto it = d2.begin();
    for(auto const val : d1) {
        ASSERT_DOUBLE_EQ(val, *it++);
    }
}

template<typename DataType>
static
std::shared_ptr<DataType> generate_time_freq_data(unsigned number_of_samples, unsigned number_of_channels)
{
    typedef typename DataType::FrequencyType FrequencyType;

    std::shared_ptr<DataType> data = std::make_shared<DataType>(number_of_samples, number_of_channels);
    FrequencyType delta( 1.0 * boost::units::si::mega * boost::units::si::hertz);
    FrequencyType start( 100.0 * boost::units::si::mega * boost::units::si::hertz);

    data->set_channel_frequencies_const_width(start, delta);

    // generate a spectrum
    return data;
}


template <typename TypeParam, typename DeviceType>
void test_rfi_algorithm(
    std::function<void (
        typename TypeParam::FlaggedDataType::TimeFrequencyFlagsType const &,
        typename TypeParam::FlaggedDataType::TimeFrequencyFlagsType const &)> assertion,
        DeviceType &device, typename TypeParam::FlaggedDataType &data)
{
    using DataType = typename TypeParam::DataType;
    using TimeFrequencyFlags = typename TypeParam::FlaggedDataType::TimeFrequencyFlagsType;

    TimeFrequencyFlags expected_flags(data.rfi_flags());

    TypeParam tester;
    std::shared_ptr<typename TypeParam::FlaggedDataType> result = tester.apply_algorithm(device, data.tf_data());

    assertion(expected_flags, result->rfi_flags());
}

namespace {
    struct TestDataWriter
    {
        public:
            TestDataWriter(std::string const& prefix, bool record_files)
                : _test_dir(!record_files)
                , _prefix(_test_dir.path() / prefix)
                , _record(record_files)
            {
                _test_dir.create();
                boost::filesystem::create_directory(_prefix);
            }

            template<typename DataType>
            void write(std::string const& test_name, DataType const& data) {
                if(_record)
                {
                    boost::filesystem::path dir(_prefix/test_name);
                    if(!boost::filesystem::create_directory(dir)) {
                        panda::Error e("unable to create directory:");
                        e << dir;
                        throw e;
                    }
                    std::cout << "writing to " << dir;
                    sigproc::SigProcWriter<> sigproc_writer(_prefix / test_name);
                    sigproc_writer << data::ExtractTimeFrequencyDataType<DataType>::extract(data);
                }
            }

        private:
            panda::test::TestDir _test_dir;
            boost::filesystem::path _prefix;
            bool _record;
    };
}

template<int Num, typename TypeParam, typename DeviceType, typename Enable = void>
struct RfiScenarioLauncher {
    typedef typename TypeParam::DataType DataType;
    inline static
    void exec(DataType const&, DeviceType&) {
        //std::string const test_name("RfiScenario<" + std::to_string(Num) + ">");
        //std::cout << "test does not exist: " << test_name << std::endl;
    }
};

template<int Num, typename TypeParam, typename DeviceType>
struct RfiScenarioLauncher<Num, TypeParam, DeviceType, typename std::enable_if<std::is_constructible<generators::RfiScenario<Num, typename TypeParam::DataType>>::value>::type >
{
    typedef typename TypeParam::DataType DataType;
    typedef typename TypeParam::FlaggedDataType FlaggedDataType;
    typedef typename FlaggedDataType::TimeFrequencyFlagsType TimeFrequencyFlags;
    typedef typename DataType::NumericalRep NumRepType;

    // apply the N'th pre-defined Rfi Scenario to the data
    inline static
    void exec(data::RfimFlaggedData<DataType> const& base_data, DeviceType& device) {

        typedef cheetah::generators::RfiScenario<Num, DataType> Scenario;
        std::string const test_name("RfiScenario<" + std::to_string(Num) + ">: " + std::string(Scenario::description));
        std::cout << "running test: " << test_name << std::endl;

        data::RfimFlaggedData<DataType> data(base_data);
        Scenario scenario;
        scenario.next(data);
        TestDataWriter file_writer(test_name, false); // DEBUG set to true to record data files during debugging (should be false otherwise)
        file_writer.write("tf_data_input", data);
        test_rfi_algorithm<TypeParam>(
            [test_name] (TimeFrequencyFlags const &expected, TimeFrequencyFlags const &given)
            {
                SCOPED_TRACE( test_name );
                rfim::Metrics m(expected, given);
                std::cout << test_name << " metrics\n"
                          << "\t" << "rfi found=" << m.num_correct() << " of " << m.num_rfi() << " (" << m.rfi_detected_percentage() << "%)\n"
                          << "\t" << "false +ve=" << m.num_false_positives() << " (" << m.false_positives_percentage() << "%)\n"
                          << "\t" << "false -ve=" << m.num_false_negatives() << " (" << m.false_negatives_percentage() << "%)\n"
                          << "\t" << "total correct=" << m.correct_percentage() << "%\n"
                          ;
                EvaluateRfimMetrics<TypeParam, Scenario>::evaluate(m);
            }, device, data);
        RfiScenarioLauncher<Num+1, TypeParam, DeviceType>::exec(data, device);
    }

};

ALGORITHM_TYPED_TEST_P(RfimTester, DISABLED_gaussian_noise_wth_rfi)
{
    // Generates a series of tests each with a gaussian noise with the RfiScenario<n> imposed on top
    // n.b RfiScenario<0> is no RFI - i.e just gaussian noise.
    typedef typename TypeParam::DataType DataType;
    typedef typename TypeParam::FlaggedDataType FlaggedDataType;
    using TimeFrequencyFlags = typename FlaggedDataType::TimeFrequencyFlagsType;
    using NumericalRep = typename DataType::NumericalRep;
    typename DataType::FrequencyType delta( 1.0 * boost::units::si::mega * boost::units::si::hertz);
    typename DataType::FrequencyType start( 100.0 * boost::units::si::mega * boost::units::si::hertz);

    generators::GaussianNoiseConfig config;
    generators::GaussianNoise<DataType> noise(config);
    std::shared_ptr<DataType> time_frequency_data(new DataType(data::DimensionSize<data::Time>(2048), data::DimensionSize<data::Frequency>(1024))); // at least one channel
    time_frequency_data->sample_interval(typename DataType::TimeType( 0.01 * boost::units::si::seconds));
    time_frequency_data->set_channel_frequencies_const_width(start, delta);
    data::RfimFlaggedData<DataType> flagged_time_frequency_data(time_frequency_data);
    auto& flags = flagged_time_frequency_data.rfi_flags();
    std::fill(flags.begin(), flags.end(), false);

    // set up a gaussian noise signal
    noise.next(*time_frequency_data);

    // execute all RFI Scenarios
    //typedef typename std::decay<decltype(device)>::type::element_type DeviceType;
    typedef decltype(device) DeviceType;
    RfiScenarioLauncher<0, TypeParam, DeviceType>::exec(flagged_time_frequency_data, device);
}

ALGORITHM_TYPED_TEST_P(RfimTester, DISABLED_single_pulse_on_gaussian_noise_wth_rfi)
{
    // Generates a series of tests each with a gaussian noise with the RfiScenario<n> imposed on top
    // n.b RfiScenario<0> is no RFI - i.e just gaussian noise.
    typedef typename TypeParam::DataType DataType;
    typedef typename TypeParam::FlaggedDataType FlaggedDataType;
    using TimeFrequencyFlags = typename FlaggedDataType::TimeFrequencyFlagsType;
    using NumericalRep = typename DataType::NumericalRep;
    typename DataType::FrequencyType delta( 1.0 * boost::units::si::mega * boost::units::si::hertz);
    typename DataType::FrequencyType start( 100.0 * boost::units::si::mega * boost::units::si::hertz);

    generators::GaussianNoiseConfig config;
    generators::DispersedPulseConfig pulse_config;
    generators::GaussianNoise<DataType> noise(config);
    generators::DispersedPulse<DataType> pulse_generator(pulse_config);

    std::shared_ptr<DataType> time_frequency_data(new DataType(data::DimensionSize<data::Time>(16384), data::DimensionSize<data::Frequency>(1024))); // at least one channel
    time_frequency_data->set_channel_frequencies_const_width(start, delta);
    time_frequency_data->sample_interval(typename DataType::TimeType( 0.01 * boost::units::si::seconds));
    data::RfimFlaggedData<DataType> flagged_time_frequency_data(time_frequency_data);

    // set up a gaussian noise signal
    noise.next(*time_frequency_data);
    pulse_generator.next(flagged_time_frequency_data);

    // execute all RFI Scenarios
    //typedef typename std::decay<decltype(device)>::type::element_type DeviceType;
    typedef decltype(device) DeviceType;
    RfiScenarioLauncher<0, TypeParam, DeviceType>::exec(flagged_time_frequency_data, device);
}

// each test defined by ALGORITHM_TYPED_TEST_P must be added to the
// test register (each one as an element of the comma seperated list)
REGISTER_TYPED_TEST_SUITE_P(RfimTester, DISABLED_gaussian_noise_wth_rfi, DISABLED_single_pulse_on_gaussian_noise_wth_rfi);

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
