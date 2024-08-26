/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#include "cheetah/data/test/CandidateTest.h"
#include "cheetah/data/Candidate.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {


CandidateTest::CandidateTest() : ::testing::Test()
{
}

CandidateTest::~CandidateTest()
{
}

void CandidateTest::SetUp()
{
}

void CandidateTest::TearDown()
{
}

// Set the input data and re-read them, no harmonic, width, or sigma (hws)
TEST_F(CandidateTest, test_input_data_no_hws)
{
    typedef Candidate<Cpu, double> CandidateType;

    // Set the pulsar_candidate period to 1.24 ms
    CandidateType::MsecTimeType period_val(0.00124 * boost::units::si::seconds);
    // Set pulsar_candidate derivative period
    typename CandidateType::SecPerSecType pdot_val(0.000001);
    // Set pulsar_candidate dispersion measure
    typename CandidateType::Dm dm_val(10. * parsecs_per_cube_cm);
    // Set pulsar_candidate ident
    std::size_t ident_val = 0;

    // Create a pulsar_candidate with the specified period, pdot, dm, and ident
    CandidateType pulsar_cand(period_val
                              , pdot_val
                              , dm_val
                             );

    // Check that the values returned from the candidate match what we set them to
    ASSERT_EQ(period_val, pulsar_cand.period());
    ASSERT_EQ(pdot_val, pulsar_cand.pdot());
    ASSERT_EQ(dm_val, pulsar_cand.dm());
    ASSERT_EQ(ident_val, pulsar_cand.ident());
}

// Set the input data and re-read them, with harmonic, width, and sigma (hws)
TEST_F(CandidateTest, test_input_data_with_hws)
{
    typedef Candidate<Cpu, double> CandidateType;

    // Set the pulsar_candidate period to 1.24 ms
    CandidateType::MsecTimeType period_val(0.00124 * boost::units::si::seconds);
    // Set pulsar_candidate derivative period
    typename CandidateType::SecPerSecType pdot_val(0.000001);
    // Set pulsar_candidate dispersion measure
    typename CandidateType::Dm dm_val(10. * parsecs_per_cube_cm);
    // Set the pulsar candidate harmonic number
    uint8_t harm_val = 1;
    // Set pulsar_candidate pulse width
    CandidateType::MsecTimeType width_val(0.0002 * boost::units::si::seconds);
    // Set pulsar_candidate signal-to-noise
    double sigma_val = 20;
    // Set pulsar_candidate ident
    std::size_t ident_val = 0;

    // Create a pulsar_candidate with the specified period, pdot, dm, and ident
    CandidateType pulsar_cand(period_val
                              , pdot_val
                              , dm_val
                              , harm_val
                              , width_val
                              , sigma_val
                             );

    // Check that the values returned from the candidate match what we set them to
    ASSERT_EQ(period_val, pulsar_cand.period());
    ASSERT_EQ(pdot_val, pulsar_cand.pdot());
    ASSERT_EQ(dm_val, pulsar_cand.dm());
    ASSERT_EQ(harm_val, pulsar_cand.harmonic());
    ASSERT_EQ(width_val, pulsar_cand.width());
    ASSERT_EQ(sigma_val, pulsar_cand.sigma());
    ASSERT_EQ(ident_val, pulsar_cand.ident());
}



// Create a zero-sized Candidate object, set data members, and re-read them (no hws)
TEST_F(CandidateTest, test_set_parameter_no_hws)
{
    typedef Candidate<Cpu, double> CandidateType;

    // Set the pulsar_candidate period to 1.24 ms
    CandidateType::MsecTimeType period_val(0.00124 * boost::units::si::seconds);
    // Set pulsar_candidate derivative period
    typename CandidateType::SecPerSecType pdot_val(0.000001);
    // Set pulsar_candidate dispersion measure
    typename CandidateType::Dm dm_val(10. * parsecs_per_cube_cm);
    // Set pulsar_candidate ident
    std::size_t ident_val = 5;

    // Create a pulsar_candidate with default values
    CandidateType pulsar_cand;

    // Assign specific values
    pulsar_cand.period(period_val);
    pulsar_cand.pdot(pdot_val);
    pulsar_cand.dm(dm_val);
    pulsar_cand.ident(ident_val);

    // Check that the values returned from the candidate match what we set them to
    ASSERT_EQ(period_val, pulsar_cand.period());
    ASSERT_EQ(pdot_val, pulsar_cand.pdot());
    ASSERT_EQ(dm_val, pulsar_cand.dm());
    ASSERT_EQ(ident_val, pulsar_cand.ident());
}

// Create a zero-sized Candidate object, set data members, and re-read them (with hws)
TEST_F(CandidateTest, test_set_parameter_with_hws)
{
    typedef Candidate<Cpu, double> CandidateType;

    // Set the pulsar_candidate period to 1.24 ms
    CandidateType::MsecTimeType period_val(0.00124 * boost::units::si::seconds);
    // Set pulsar_candidate derivative period
    typename CandidateType::SecPerSecType pdot_val(0.000001);
    // Set pulsar_candidate dispersion measure
    typename CandidateType::Dm dm_val(10. * parsecs_per_cube_cm);
    // Set the pulsar candidate harmonic number
    uint8_t harm_val = 1;
    // Set pulsar_candidate pulse width
    CandidateType::MsecTimeType width_val(0.0002 * boost::units::si::seconds);
    // Set pulsar_candidate signal-to-noise
    double sigma_val = 20;
    // Set pulsar_candidate ident
    std::size_t ident_val = 5;

    // Create a pulsar_candidate with default values
    CandidateType pulsar_cand;

    // Assign specific values
    pulsar_cand.period(period_val);
    pulsar_cand.pdot(pdot_val);
    pulsar_cand.dm(dm_val);
    pulsar_cand.harmonic(harm_val);
    pulsar_cand.width(width_val);
    pulsar_cand.sigma(sigma_val);
    pulsar_cand.ident(ident_val);

    // Check that the values returned from the candidate match what we set them to
    ASSERT_EQ(period_val, pulsar_cand.period());
    ASSERT_EQ(pdot_val, pulsar_cand.pdot());
    ASSERT_EQ(dm_val, pulsar_cand.dm());
    ASSERT_EQ(harm_val, pulsar_cand.harmonic());
    ASSERT_EQ(width_val, pulsar_cand.width());
    ASSERT_EQ(sigma_val, pulsar_cand.sigma());
    ASSERT_EQ(ident_val, pulsar_cand.ident());
}



// Create a Candidate object, make a copy, and compare the members values (no hws)
TEST_F(CandidateTest, test_candidate_copy_no_hws)
{
    typedef Candidate<Cpu, double> CandidateType;

    // Set the pulsar_candidate period to 1.24 ms
    CandidateType::MsecTimeType period_val(0.00124 * boost::units::si::seconds);
    // Set pulsar_candidate derivative period
    typename CandidateType::SecPerSecType pdot_val(0.000001);
    // Set pulsar_candidate dispersion measure
    typename CandidateType::Dm dm_val(10. * parsecs_per_cube_cm);
    // Set pulsar_candidate ident
    std::size_t ident_val = 5;

    // Create a pulsar_candidate with configured values
    CandidateType pulsar_cand(period_val
                              , pdot_val
                              , dm_val
                              , ident_val
                             );

    // Copy candidate
    CandidateType pulsar_cand_copy(pulsar_cand);

    // Compare the two candidates
    ASSERT_EQ(pulsar_cand_copy.period(), pulsar_cand.period());
    ASSERT_EQ(pulsar_cand_copy.pdot(), pulsar_cand.pdot());
    ASSERT_EQ(pulsar_cand_copy.dm(), pulsar_cand.dm());
    ASSERT_EQ(pulsar_cand_copy.ident(), pulsar_cand.ident());
}

// Create a Candidate object, make a copy, and compare the members values (with hws)
TEST_F(CandidateTest, test_candidate_copy_with_hws)
{
    typedef Candidate<Cpu, double> CandidateType;

    // Set the pulsar_candidate period to 1.24 ms
    CandidateType::MsecTimeType period_val(0.00124 * boost::units::si::seconds);
    // Set pulsar_candidate derivative period
    typename CandidateType::SecPerSecType pdot_val(0.000001);
    // Set pulsar_candidate dispersion measure
    typename CandidateType::Dm dm_val(10. * parsecs_per_cube_cm);
    // Set pulsar_candidate harmonic number
    uint8_t harm_val = 1;
    // Set the pulsar_candidate pulse width
    CandidateType::MsecTimeType width_val(0.0002 * boost::units::si::seconds);
    // Set pulsar_candidate signal-to-noise
    double sigma_val = 20;
    // Set pulsar_candidate ident
    std::size_t ident_val = 5;

    // Create a pulsar_candidate with configured values
    CandidateType pulsar_cand(period_val
                              , pdot_val
                              , dm_val
                              , harm_val
                              , width_val
                              , sigma_val
                              , ident_val
                             );

    // Copy candidate
    CandidateType pulsar_cand_copy(pulsar_cand);

    // Compare the two candidates
    ASSERT_EQ(pulsar_cand_copy.period(), pulsar_cand.period());
    ASSERT_EQ(pulsar_cand_copy.pdot(), pulsar_cand.pdot());
    ASSERT_EQ(pulsar_cand_copy.dm(), pulsar_cand.dm());
    ASSERT_EQ(pulsar_cand_copy.harmonic(), pulsar_cand.harmonic());
    ASSERT_EQ(pulsar_cand_copy.width(), pulsar_cand.width());
    ASSERT_EQ(pulsar_cand_copy.sigma(), pulsar_cand.sigma());
    ASSERT_EQ(pulsar_cand_copy.ident(), pulsar_cand.ident());
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
