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
#include "cheetah/generators/test/PulsarInjectionConfigTest.h"
#include "cheetah/generators/PulsarInjectionConfig.h"
#include "panda/test/gtest.h"

namespace ska {
namespace cheetah {
namespace generators {
namespace test {


PulsarInjectionConfigTest::PulsarInjectionConfigTest()
    : ::testing::Test()
{
}

PulsarInjectionConfigTest::~PulsarInjectionConfigTest()
{
}

void PulsarInjectionConfigTest::SetUp()
{
}

void PulsarInjectionConfigTest::TearDown()
{
}

TEST_F(PulsarInjectionConfigTest, test_simple_phase_model)
{
    // test the sinple phase configuration propogates to the model
    // we test this by ensuring the generated models generate different results
    PulsarInjectionConfig config_a;
    SimplePhaseModelConfig simple_config_a;
    simple_config_a.coefficients( {1.0} );
    config_a.set_phase_model(simple_config_a);
    auto model_a = config_a.phase_model();

    PulsarInjectionConfig config_b;
    SimplePhaseModelConfig simple_config_b;
    simple_config_b.coefficients( {5.0, 3.0} );
    config_b.set_phase_model(simple_config_b);
    auto model_b = config_b.phase_model();

    utils::ModifiedJulianClock::time_point mjd = utils::ModifiedJulianClock::now();
    
    ASSERT_NE(model_a(mjd, 100 * boost::units::si::mega * boost::units::si::hertz),
              model_b(mjd, 100 * boost::units::si::mega * boost::units::si::hertz) );
}

TEST_F(PulsarInjectionConfigTest, test_tempo2_phase_model)
{
    PulsarInjectionConfig config;
    Tempo2PhaseModelConfig tempo2_config;
    tempo2_config.filename(panda::test::test_file("tempo2_psr_a.pred"));
    config.set_phase_model(tempo2_config);

    config.phase_model();
}

} // namespace test
} // namespace generators
} // namespace cheetah
} // namespace ska
