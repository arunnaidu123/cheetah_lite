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
#include "cheetah/data/test/SclTest.h"
#include "cheetah/data/Scl.h"
#include "cheetah/data/Ccl.h"
#include "cheetah/data/Candidate.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {


SclTest::SclTest() : ::testing::Test()
{
}

SclTest::~SclTest()
{
}

void SclTest::SetUp()
{
}

void SclTest::TearDown()
{
}

TEST_F(SclTest, test_construction)
{
    //set candidate dispersion measure
    typename Scl::CandidateType::Dm dm = 10 * parsecs_per_cube_cm;
    //set candidate ident
    std::size_t ident = 4;
    //set the candidate width to 1.24 ms
    typename Scl::CandidateType::MsecTimeType width(0.00124 * boost::units::si::seconds);
    //set the period to 10.0 seconds
    typename Scl::CandidateType::MsecTimeType period(10.0 * boost::units::si::seconds);
    //set the harmonic number
    uint8_t harmonic = 1;
    //set the candidate significance
    double sigma = 15.0;
    // set the candidate P-dot
    typename Scl::CandidateType::SecPerSecType pdot = 1e-11;

    // generate a candidate with the specified parameters
    typename Scl::CandidateType candidate(period, pdot, dm, harmonic, width, sigma, ident) ;

    // generate a Ccl object
    ASSERT_NO_THROW(Scl candidate_list);
}


TEST_F(SclTest, test_candidate_vector_behaviour)
{
    typedef typename Scl::CandidateType CandidateType;
    std::size_t idx;

    //Create new Scl<NumericalRep> instance
    Scl cand_list;

    //set candidate dispersion measure
    typename CandidateType::Dm dm(15.0 * parsecs_per_cube_cm);
    //set the candidate width to 1.0 ms
    CandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the start time to 2.0 seconds
    CandidateType::MsecTimeType period(2.0 * boost::units::si::seconds);
    //set the harmonic number
    uint8_t harmonic = 1;
    //set the pdot
    typename CandidateType::SecPerSecType pdot = 1e-13;
    //set the candidate significance
    float sigma = 20.0;

    for (idx=0; idx<10; ++idx)
    {
        CandidateType candidate(period, pdot, dm, harmonic, width, sigma, idx);
        cand_list.push_back(candidate);
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    for (idx=0; idx<10; ++idx)
    {
        auto const& cand = cand_list[idx];
        ASSERT_EQ(cand.dm(), dm);
        ASSERT_EQ(cand.period(), period);
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand.ident(), idx);
    }

    Scl cand_list2;

    for (idx=0; idx<10; ++idx)
    {
        CandidateType candidate(period, pdot, dm, harmonic, width, sigma, idx);
        cand_list2.emplace_back(std::move(candidate));
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    for (idx=0; idx<10; ++idx)
    {
        auto const& cand = cand_list2[idx];
        ASSERT_EQ(cand.dm(), dm);
        ASSERT_EQ(cand.period(), period);
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand.ident(), idx);
    }
}

TEST_F(SclTest, test_disordered_input_sort_behaviour)
{

    typedef typename Scl::CandidateType CandidateType;


    //Create new Scl instance
    Scl cand_list;
    Scl cand_list2;

    //set candidate dispersion measure
    typename CandidateType::Dm dm(23 * parsecs_per_cube_cm);

    typename CandidateType::MsecTimeType width(0.00332 * boost::units::si::seconds);

    //set the candidate period
    CandidateType::MsecTimeType period(432.3332 * boost::units::si::seconds);

    //set the harmonic number
    uint8_t harmonic = 1;

    //set the pdot
    typename CandidateType::SecPerSecType pdot = 1e-15;

    //set the candidate significance
    float sigma = 20.0;

    for (std::size_t idx=0; idx<10; ++idx)
    {
        CandidateType candidate(period, pdot, dm, harmonic, width, sigma, idx);
        cand_list.push_back(candidate);
        cand_list2.emplace_back(std::move(candidate));
        dm -= CandidateType::Dm(1.0*parsecs_per_cube_cm);
    }

    // real check
    ASSERT_EQ(cand_list.size(), std::size_t(10));

    typename CandidateType::Dm sdm(14 * parsecs_per_cube_cm);
    for (std::size_t idx=0; idx<10; ++idx)
    {
        SCOPED_TRACE(idx);
        auto const& cand = cand_list[idx];
        auto const& cand2 = cand_list2[idx];
        ASSERT_EQ(cand.dm(), sdm);
        ASSERT_EQ(cand2.dm(), sdm);
        ASSERT_EQ(cand.width(), width);
        ASSERT_EQ(cand2.width(), width);
        ASSERT_EQ(cand.sigma(), sigma);
        ASSERT_EQ(cand2.sigma(), sigma);
        ASSERT_EQ(cand.ident(), 9-idx);
        ASSERT_EQ(cand2.ident(), 9-idx);
        sdm += CandidateType::Dm(1.0*parsecs_per_cube_cm);
    }
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
