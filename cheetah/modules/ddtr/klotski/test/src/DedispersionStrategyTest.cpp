/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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

#include "cheetah/modules/ddtr/klotski/test/DedispersionStrategyTest.h"
#include "cheetah/modules/ddtr/klotski/detail/DedispersionStrategy.h"
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include <pss/astrotypes/units/Units.h>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {
namespace test {

template<typename T>
DedispersionStrategyTest<T>::DedispersionStrategyTest()
    : ::testing::Test()
{
}

template<typename T>
DedispersionStrategyTest<T>::~DedispersionStrategyTest()
{
}

typedef ::testing::Types<unsigned char, unsigned short, float> NumericTypes;
TYPED_TEST_SUITE(DedispersionStrategyTest, NumericTypes);

/**
 * @test test_DedispersionStrategy_constructor
 * @given DedispersionStrategy class
 * @when contructed with right plan and tf objects
 * @then object must constructed as expected
*/
TYPED_TEST(DedispersionStrategyTest, test_DedispersionStrategy_constructor)
{

    typedef TypeParam NumericRep;

    data::DimensionSize<data::Time> number_of_spectra(128*1024U);
    data::DimensionSize<data::Frequency> number_of_channels(512U);

    data::TimeFrequency<Cpu, NumericRep> tf(number_of_spectra, number_of_channels);

    tf.set_channel_frequencies_const_width
                        (data::FrequencyType(1450.0 * data::megahertz)
                        , data::FrequencyType(-0.08056640625 * data::megahertz)
                        );

    tf.sample_interval(0.001*data::second);
    ddtr::Config plan;
    plan.dedispersion_samples(1<<14);
    ASSERT_EQ(1<<14, plan.dedispersion_samples());

    ASSERT_DOUBLE_EQ(0.0, plan.max_dm().value());

    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 40 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm); // should not affect the max dm
    ASSERT_DOUBLE_EQ(39.0, plan.max_dm().value());


    DedispersionStrategy<NumericRep> ds(tf, plan,128*1024*1024);

    ASSERT_EQ(ds.nchans(), 512);
    ASSERT_EQ(ds.nsamps(), plan.dedispersion_samples());
    ASSERT_EQ(ds.dm_low()[0], 0.0*data::parsecs_per_cube_cm);
    ASSERT_EQ(ds.dm_high()[0], 40.0*data::parsecs_per_cube_cm);
    ASSERT_EQ(ds.tsamp()[0].value(), 0.001);

}

/**
 * @test test_DedispersionStrategy_throw
 * @given DedispersionStrategy class
 * @when when contructed with dm ranges with very large dm values
 * @then contruction of the object is expected to throw
 */
TYPED_TEST(DedispersionStrategyTest, test_DedispersionStrategy_throw)
{

    typedef TypeParam NumericRep;

    data::DimensionSize<data::Time> number_of_spectra(128*1024U);
    data::DimensionSize<data::Frequency> number_of_channels(512U);

    data::TimeFrequency<Cpu, NumericRep> tf(number_of_spectra, number_of_channels);
    tf.set_channel_frequencies_const_width
                        (data::FrequencyType(1450.0 * data::megahertz)
                        , data::FrequencyType(-0.08056640625 * data::megahertz)
                        );
    tf.sample_interval(0.0001*data::second);
    ddtr::Config config;
    config.dedispersion_samples(1<<14);
    ASSERT_EQ(1<<14, config.dedispersion_samples());
    ASSERT_DOUBLE_EQ(0.0, config.max_dm().value());

    config.add_dm_range(0 * data::parsecs_per_cube_cm, 4000 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm); // should not affect the max dm
    ASSERT_DOUBLE_EQ(3999.0, config.max_dm().value());

    config.add_dm_range(4000 * data::parsecs_per_cube_cm, 10000 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm);
    ASSERT_DOUBLE_EQ(9999.0, config.max_dm().value());

    typedef DedispersionStrategy<NumericRep> DsType;
    ASSERT_THROW(DsType ds(tf, config, 128*1024*1024), panda::Error);

}


} // namespace test
} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
