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
#include "cheetah/data/test/SpCandidateTest.h"
#include "cheetah/data/SpCandidate.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {


SpCandidateTest::SpCandidateTest() : ::testing::Test()
{
}

SpCandidateTest::~SpCandidateTest()
{
}

void SpCandidateTest::SetUp()
{
}

void SpCandidateTest::TearDown()
{
}

// test_input_data
// Sets the input data and re-read them
//
TEST_F(SpCandidateTest, test_input_data)
{
    typedef SpCandidate<Cpu, double> SpCandidateType;

    //set single pulse candidate dispersion measure
    typename SpCandidateType::Dm dm = 10. * parsecs_per_cube_cm;
    //set single pulse candidate ident
    std::size_t ident = 4;
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(0.00124 * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(2.23 * boost::units::si::seconds);
    //set the candidate significance
    double sigma = 15.0;
    SpCandidateType::FrequencyType f_high(1700 * boost::units::si::mega * boost::units::si::hertz);
    SpCandidateType::FrequencyType f_low(1400 *boost::units::si::mega * boost::units::si::hertz);

    // generate a single pulse candidate with the specified parameters
    SpCandidateType candidate(dm, tstart, width, sigma, ident) ;
    ASSERT_NO_THROW(candidate.dm_with_duration_end(f_high,f_low,dm));
    // real check
    ASSERT_EQ(dm, candidate.dm());
    ASSERT_EQ(tstart, candidate.tstart());
    ASSERT_EQ(width, candidate.width());
    ASSERT_EQ(sigma, candidate.sigma());
    ASSERT_EQ(ident, candidate.ident());
}

TEST_F(SpCandidateTest, test_candidate_copy)
{
    typedef SpCandidate<Cpu, double> SpCandidateType;

    //set single pulse candidate dispersion measure
    typename SpCandidateType::Dm dm = 10. * parsecs_per_cube_cm;
    //set single pulse candidate ident
    std::size_t ident = 4;
    //set the single pulse candidate width to 1.24 ms
    SpCandidateType::MsecTimeType width(0.00124 * boost::units::si::seconds);
    //set the single pulse start time to 2.23 seconds
    SpCandidateType::MsecTimeType tstart(2.23 * boost::units::si::seconds);
    //set the candidate significance
    double sigma = 15.0;
    //set the end time
    SpCandidateType::MsecTimeType tend(2000 * boost::units::si::milli * boost::units::si::seconds);

    // generate a single pulse candidate with the specified parameters
    SpCandidateType candidate(dm, tstart, width, sigma, ident);

    // set tend
    candidate.duration_end(tend);
    // copy candidate
    SpCandidateType candidate_copy(candidate);

    // real check
    ASSERT_EQ(dm, candidate_copy.dm());
    ASSERT_EQ(tstart, candidate_copy.tstart());
    ASSERT_EQ(width, candidate_copy.width());
    ASSERT_EQ(sigma, candidate_copy.sigma());
    ASSERT_EQ(ident, candidate_copy.ident());
    ASSERT_EQ(tend + tstart, candidate_copy.tend());
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
