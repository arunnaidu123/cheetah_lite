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
#include "cheetah/modules/ddtr/test/DedispersionTrialPlanTest.h"
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include "panda/Log.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


DedispersionTrialPlanTest::DedispersionTrialPlanTest()
    : ::testing::Test()
{
}

DedispersionTrialPlanTest::~DedispersionTrialPlanTest()
{
}

void DedispersionTrialPlanTest::SetUp()
{
}

void DedispersionTrialPlanTest::TearDown()
{
}

TEST_F(DedispersionTrialPlanTest, test_dm_constant)
{
    DedispersionTrialPlan plan("test");
    ASSERT_DOUBLE_EQ(plan.dm_constant().value(), data::dm_constant::s_mhz::dm_constant.value());
}

TEST_F(DedispersionTrialPlanTest, test_max_dm)
{
    DedispersionTrialPlan plan("test");
    ASSERT_DOUBLE_EQ(0.0, plan.max_dm().value());

    plan.add_dm_range(40 * data::parsecs_per_cube_cm, 100 * data::parsecs_per_cube_cm, 20 * data::parsecs_per_cube_cm);
    ASSERT_DOUBLE_EQ(80.0, plan.max_dm().value());

    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 40 * data::parsecs_per_cube_cm, 20 * data::parsecs_per_cube_cm); // should not affect the max dm
    ASSERT_DOUBLE_EQ(80.0, plan.max_dm().value());

    plan.add_dm_range(400 * data::parsecs_per_cube_cm, 500 * data::parsecs_per_cube_cm, 10 * data::parsecs_per_cube_cm);
    ASSERT_DOUBLE_EQ(490.0, plan.max_dm().value());

    // should also work with dedispersion_config objects directly
    DedispersionConfig dd_config(800 * data::parsecs_per_cube_cm, 900 * data::parsecs_per_cube_cm, 50 * data::parsecs_per_cube_cm);
    plan.dedispersion_config(dd_config);
    ASSERT_DOUBLE_EQ(850.0, plan.max_dm().value());
}

TEST_F(DedispersionTrialPlanTest, test_maximum_delay)
{
    DedispersionTrialPlan plan("test");
    plan.add_dm_range(40 * data::parsecs_per_cube_cm, 100 * data::parsecs_per_cube_cm, 20 * data::parsecs_per_cube_cm);
    ASSERT_EQ(3, plan.dm_trials().size());

    boost::units::quantity<data::MegaHertz, double> low(100.0 * boost::units::si::mega * boost::units::si::hertz);
    boost::units::quantity<data::MegaHertz, double> high(300.0 * boost::units::si::mega * boost::units::si::hertz);
    boost::units::quantity<boost::units::si::time, double> delay =  plan.maximum_delay(low, high);
    ASSERT_DOUBLE_EQ(29.506685108344183, delay.value());

}

TEST_F(DedispersionTrialPlanTest, test_maximum_delay_offset)
{
    DedispersionTrialPlan plan("test");
    plan.add_dm_range(40 * data::parsecs_per_cube_cm, 120 * data::parsecs_per_cube_cm, 20 * data::parsecs_per_cube_cm);

    data::TimeFrequency<Cpu, uint16_t> tf(data::DimensionSize<data::Frequency>(100), data::DimensionSize<data::Time>(100));
    typedef typename data::TimeFrequency<Cpu, uint16_t>::TimeType TimeType;
    tf.sample_interval( TimeType(100.0 * boost::units::si::milli * boost::units::si::seconds) );
    tf.set_channel_frequencies_const_width(100.0 * boost::units::si::mega * boost::units::si::hertz, 2.0 * boost::units::si::mega * boost::units::si::hertz);
    auto offset = plan.maximum_delay_offset(tf);
    ASSERT_EQ(offset,369);
}

TEST_F(DedispersionTrialPlanTest, test_generate_dmtrials_metadata_one_range)
{
    DedispersionTrialPlan plan("test");
    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 10 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm);

    data::TimeFrequency<Cpu, uint16_t> tf(data::DimensionSize<data::Frequency>(77), data::DimensionSize<data::Time>(131072));
    typedef typename data::TimeFrequency<Cpu, uint16_t>::TimeType TimeType;
    tf.sample_interval( TimeType(34.567 * boost::units::si::micro * boost::units::si::seconds) );
    tf.set_channel_frequencies_const_width(350.0 * boost::units::si::mega * boost::units::si::hertz, -0.028935 * boost::units::si::mega * boost::units::si::hertz);
    std::size_t nspectra = tf.number_of_spectra();
    data::DimensionSize<data::Time> overlap = plan.maximum_delay_offset(tf);
    ASSERT_LT(overlap, nspectra);
    std::shared_ptr<data::DmTrialsMetadata> dm_trials_metadata;
    dm_trials_metadata = plan.generate_dmtrials_metadata(tf.sample_interval(), nspectra, overlap);
    std::size_t number_dm_trial_values = 10;
    ASSERT_EQ(dm_trials_metadata->size(), number_dm_trial_values);
    ASSERT_EQ(tf.sample_interval(), dm_trials_metadata->fundamental_sampling_interval());
    ASSERT_EQ(dm_trials_metadata->duration(), (nspectra - overlap) * tf.sample_interval());
}

TEST_F(DedispersionTrialPlanTest, test_generate_dmtrials_metadata_two_ranges)
{
    DedispersionTrialPlan plan("test");
    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 10 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm);
    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 101 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm);

    data::TimeFrequency<Cpu, uint16_t> tf(data::DimensionSize<data::Frequency>(77), data::DimensionSize<data::Time>(670400));
    typedef typename data::TimeFrequency<Cpu, uint16_t>::TimeType TimeType;
    tf.sample_interval( TimeType(34.567 * boost::units::si::micro * boost::units::si::seconds) );
    tf.set_channel_frequencies_const_width(350.0 * boost::units::si::mega * boost::units::si::hertz, -0.028935 * boost::units::si::mega * boost::units::si::hertz);
    std::size_t nspectra = tf.number_of_spectra();
    data::DimensionSize<data::Time> overlap = plan.maximum_delay_offset(tf);

    ASSERT_LT(overlap, nspectra);
    std::shared_ptr<data::DmTrialsMetadata> dm_trials_metadata;
    dm_trials_metadata = plan.generate_dmtrials_metadata(tf.sample_interval(), nspectra, overlap);
    std::size_t number_dm_trial_values = 111;
    ASSERT_EQ(dm_trials_metadata->size(), number_dm_trial_values);
    ASSERT_EQ(tf.sample_interval(), dm_trials_metadata->fundamental_sampling_interval());
    ASSERT_EQ(dm_trials_metadata->duration(), (nspectra - overlap) * tf.sample_interval());
}

TEST_F(DedispersionTrialPlanTest, test_generate_dmtrials_metadata_exception_catch)
{
    DedispersionTrialPlan plan("test");
    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 10 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm);

    data::TimeFrequency<Cpu, uint16_t> tf(data::DimensionSize<data::Frequency>(7776), data::DimensionSize<data::Time>(27000));
    typedef typename data::TimeFrequency<Cpu, uint16_t>::TimeType TimeType;
    tf.sample_interval( TimeType(34.567 * boost::units::si::micro * boost::units::si::seconds) );
    tf.set_channel_frequencies_const_width(350.0 * boost::units::si::mega * boost::units::si::hertz, -0.028935 * boost::units::si::mega * boost::units::si::hertz);
    std::size_t nspectra = tf.number_of_spectra();
    data::DimensionSize<data::Time> overlap = plan.maximum_delay_offset(tf);
    ASSERT_GT(overlap, nspectra);
    ASSERT_THROW(plan.generate_dmtrials_metadata(tf.sample_interval(), nspectra, overlap), panda::Error);
}

TEST_F(DedispersionTrialPlanTest, test_generate_dmtrials_metadata_no_exception)
{
    DedispersionTrialPlan plan("test");
    plan.add_dm_range(0 * data::parsecs_per_cube_cm, 10 * data::parsecs_per_cube_cm, 1 * data::parsecs_per_cube_cm);

    data::TimeFrequency<Cpu, uint16_t> tf(data::DimensionSize<data::Frequency>(7776), data::DimensionSize<data::Time>(131072));
    typedef typename data::TimeFrequency<Cpu, uint16_t>::TimeType TimeType;
    tf.sample_interval( TimeType(34.567 * boost::units::si::micro * boost::units::si::seconds) );
    tf.set_channel_frequencies_const_width(350.0 * boost::units::si::mega * boost::units::si::hertz, -0.028935 * boost::units::si::mega * boost::units::si::hertz);
    std::size_t nspectra = tf.number_of_spectra();
    data::DimensionSize<data::Time> overlap = plan.maximum_delay_offset(tf);
    ASSERT_LT(overlap, nspectra);
    ASSERT_NO_THROW(plan.generate_dmtrials_metadata(tf.sample_interval(), nspectra, overlap));
}

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
