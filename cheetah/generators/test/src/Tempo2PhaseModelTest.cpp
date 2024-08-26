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
#include "cheetah/generators/test/Tempo2PhaseModelTest.h"
#include "cheetah/generators/src/Tempo2PhaseModel.h"
#include "panda/test/gtest.h"


namespace ska {
namespace cheetah {
namespace generators {
namespace test {


Tempo2PhaseModelTest::Tempo2PhaseModelTest()
    : ::testing::Test()
{
}

Tempo2PhaseModelTest::~Tempo2PhaseModelTest()
{
}

void Tempo2PhaseModelTest::SetUp()
{
}

void Tempo2PhaseModelTest::TearDown()
{
}

TEST_F(Tempo2PhaseModelTest, test_run_uninitialised)
{
    Tempo2PhaseModelConfig config;
    ASSERT_THROW(Tempo2PhaseModel model(config), panda::Error);
}

TEST_F(Tempo2PhaseModelTest, test_run_file_does_not_exist)
{
    Tempo2PhaseModelConfig config;
    config.filename("i_do_not_exist");
    ASSERT_THROW(Tempo2PhaseModel model(config), panda::Error);
}

TEST_F(Tempo2PhaseModelTest, test_run_from_pred_file)
{
    Tempo2PhaseModelConfig config;
    config.filename(panda::test::test_file("tempo2_psr_a.pred"));
    Tempo2PhaseModel model(config);

    Tempo2PhaseModelConfig config_b;
    config_b.filename(panda::test::test_file("tempo2_psr_b.pred"));
    Tempo2PhaseModel model_b(config_b);

    utils::ModifiedJulianClock::time_point mjd(utils::julian_days(56000)); // must coincide with the model range
    auto freq = 100.0 * boost::units::si::mega * boost::units::si::hertz;
    ASSERT_NE(model(mjd, freq), model_b(mjd, freq));
}

} // namespace test
} // namespace generators
} // namespace cheetah
} // namespace ska
