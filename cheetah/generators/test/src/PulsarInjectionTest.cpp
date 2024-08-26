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
#include "cheetah/generators/test/PulsarInjectionTest.h"
#include "cheetah/generators/PulsarInjection.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"


namespace ska {
namespace cheetah {
namespace generators {
namespace test {


PulsarInjectionTest::PulsarInjectionTest()
    : ::testing::Test()
{
}

PulsarInjectionTest::~PulsarInjectionTest()
{
}

void PulsarInjectionTest::SetUp()
{
}

void PulsarInjectionTest::TearDown()
{
}

TEST_F(PulsarInjectionTest, test_no_phase_model)
{
    typedef typename data::TimeFrequency<cheetah::Cpu, uint8_t> TimeFrequencyType;
    data::DimensionSize<data::Frequency> nchans(4096);
    data::DimensionSize<data::Time> nsamples(0);
    TimeFrequencyType data(nchans, nsamples);

    PulsarInjectionConfig config;

    std::unique_ptr<generators::PulsarInjection<TimeFrequencyType>> injector;
    ASSERT_THROW(injector.reset(new generators::PulsarInjection<TimeFrequencyType>(config, _manager)), panda::Error);
}

TEST_F(PulsarInjectionTest, test_empty_data)
{
    data::DimensionSize<data::Frequency> nchans(4096);
    data::DimensionSize<data::Time> nsamples(0);
    typedef typename data::TimeFrequency<cheetah::Cpu, uint8_t> TimeFrequencyType;
    TimeFrequencyType data(nchans, nsamples);

    PulsarInjectionConfig config;
    SimplePhaseModelConfig phase_model_config;
    config.set_phase_model(phase_model_config);

    generators::PulsarInjection<TimeFrequencyType> injector(config, _manager);
    injector.next(data);
}

TEST_F(PulsarInjectionTest, test_generated_profile)
{
    typedef typename data::TimeFrequency<cheetah::Cpu, uint8_t> TimeFrequencyType;
    data::DimensionSize<data::Frequency> nchans(4096);
    data::DimensionSize<data::Time> nsamples(10000);
    TimeFrequencyType data(nchans, nsamples);
    data.set_channel_frequencies_const_width(1592.0 * boost::units::si::mega * boost::units::si::hertz,
                                             data::TimeFrequency<Cpu, uint8_t>::FrequencyType(70.0 * boost::units::si::hertz));

    PulsarInjectionConfig config;
    SimplePhaseModelConfig phase_model_config;
    phase_model_config.coefficients({1, 1}); // simple periodix model
    config.set_phase_model(phase_model_config);
    generators::PulsarInjection<TimeFrequencyType> injector(config, _manager);
    injector.next(data);
}

} // namespace test
} // namespace generators
} // namespace cheetah
} // namespace ska
