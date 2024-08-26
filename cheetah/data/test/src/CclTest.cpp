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
#include "cheetah/data/test/CclTest.h"
#include "cheetah/data/Ccl.h"
#include "cheetah/data/Candidate.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

CclTest::CclTest() : ::testing::Test()
{
}

CclTest::~CclTest()
{
}

void CclTest::SetUp()
{
}

void CclTest::TearDown()
{
}

TEST_F(CclTest, test_empty_construction)
{
    // Construct an empty Ccl object
    Ccl ccl;
    ASSERT_EQ(ccl.size(), 0);
}

TEST_F(CclTest, test_adding_candidate)
{
    typedef typename Ccl::CandidateType CandType;

    // Set the period
    CandType::MsecTimeType period(142.7 * data::milliseconds);
    // Set the candidate pdot
    CandType::SecPerSecType pdot(1e-11);
    // Set candidate dispersion measure
    CandType::Dm dm(52 * parsecs_per_cube_cm);
    // Set the candidate harmonic number
    std::size_t harmonic = 1;
    // Set the candidate width
    CandType::MsecTimeType width(1.24 * data::milliseconds);
    // Set the candidate significance
    double sigma = 9.0;
    // Set candidate number
    std::size_t ident = 2;

    // Generate a candidate with the specified parameters
    CandType candidate(period, pdot, dm, harmonic, width, sigma, ident);

    // Construct a Ccl object
    Ccl ccl;

    // Add the candidate to the Ccl
    ASSERT_NO_THROW(ccl.add(candidate));
    ASSERT_EQ(ccl.size(), 1);

}

TEST_F(CclTest, test_emplacing_candidate)
{
    typedef typename Ccl::CandidateType CandType;

    // Set the period
    CandType::MsecTimeType period(142.7 * data::milliseconds);
    // Set the candidate pdot
    CandType::SecPerSecType pdot(1e-11);
    // Set candidate dispersion measure
    CandType::Dm dm(52 * parsecs_per_cube_cm);
    // Set the candidate harmonic number
    std::size_t harmonic = 1;
    // Set the candidate width
    CandType::MsecTimeType width(1.24 * data::milliseconds);
    // Set the candidate significance
    double sigma = 9.0;
    // Set candidate number
    std::size_t ident = 2;

    // Generate a candidate with the specified parameters
    CandType candidate(period, pdot, dm, harmonic, width, sigma, ident);

    // Construct a Ccl object
    Ccl ccl;

    // Add the candidate to the Ccl
    ASSERT_NO_THROW(ccl.emplace(std::move(candidate)));
    ASSERT_EQ(ccl.size(), 1);

}

TEST_F(CclTest, test_adding_multiple_candidates)
{
    typedef typename Ccl::CandidateType CandType;

    // Set the period for the first candidate
    CandType::MsecTimeType period1(142.7 * data::milliseconds);
    // Set the pdot for the first candidate
    CandType::SecPerSecType pdot1(1e-11);
    // Set the dispersion measure for the first candidate
    CandType::Dm dm1(52 * parsecs_per_cube_cm);
    // Set the harmonic number of the first candidate
    std::size_t harmonic1 = 1;
    // Set the width for the first candidate
    CandType::MsecTimeType width1(1.24 * data::milliseconds);
    // Set the significance for the first candidate
    double sigma1 = 9.0;
    // Set the candidate number for the first candidate
    std::size_t ident1 = 1;

    // Set the period for the second candidate
    CandType::MsecTimeType period2(342.9 * data::milliseconds);
    // Set the pdot for the second candidate
    CandType::SecPerSecType pdot2(1e-15);
    // Set the dispersion measure for the second candidate
    CandType::Dm dm2(13 * parsecs_per_cube_cm);
    // Set the harmonic number of the second candidate
    std::size_t harmonic2 = 1;
    // Set the width for the second candidate
    CandType::MsecTimeType width2(17.19 * data::milliseconds);
    // Set the significance for the second candidate
    double sigma2 = 7.0;
    // Set the candidate number for the second candidate
    std::size_t ident2 = 2;

    // Set the period for the third candidate
    CandType::MsecTimeType period3(986.17 * data::milliseconds);
    // Set the pdot for the third candidate
    CandType::SecPerSecType pdot3(1e-13);
    // Set the dispersion measure for the third candidate
    CandType::Dm dm3(192 * parsecs_per_cube_cm);
    // Set the harmonic number of the third candidate
    std::size_t harmonic3 = 1;
    // Set the width for the third candidate
    CandType::MsecTimeType width3(102.1 * data::milliseconds);
    // Set the significance for the third candidate
    double sigma3 = 11.0;
    // Set the candidate number for the third candidate
    std::size_t ident3 = 3;

    // Generate candidates with the specified parameters
    CandType candidate1(period1, pdot1, dm1, harmonic1, width1, sigma1, ident1);
    CandType candidate2(period2, pdot2, dm2, harmonic2, width2, sigma2, ident2);
    CandType candidate3(period3, pdot3, dm3, harmonic3, width3, sigma3, ident3);

    // Construct a Ccl object
    Ccl ccl;

    // Add the candidates to the Ccl
    ASSERT_NO_THROW(ccl.add(candidate1));
    ASSERT_NO_THROW(ccl.add(candidate2));
    ASSERT_NO_THROW(ccl.add(candidate3));
    ASSERT_EQ(ccl.size(), 3);

}

TEST_F(CclTest, test_emplacing_multiple_candidates)
{
    typedef typename Ccl::CandidateType CandType;

    // Set the period for the first candidate
    CandType::MsecTimeType period1(142.7 * data::milliseconds);
    // Set the pdot for the first candidate
    CandType::SecPerSecType pdot1(1e-11);
    // Set the dispersion measure for the first candidate
    CandType::Dm dm1(52 * parsecs_per_cube_cm);
    // Set the harmonic number of the first candidate
    std::size_t harmonic1 = 1;
    // Set the width for the first candidate
    CandType::MsecTimeType width1(1.24 * data::milliseconds);
    // Set the significance for the first candidate
    double sigma1 = 9.0;
    // Set the candidate number for the first candidate
    std::size_t ident1 = 1;

    // Set the period for the second candidate
    CandType::MsecTimeType period2(342.9 * data::milliseconds);
    // Set the pdot for the second candidate
    CandType::SecPerSecType pdot2(1e-15);
    // Set the dispersion measure for the second candidate
    CandType::Dm dm2(13 * parsecs_per_cube_cm);
    // Set the harmonic number of the second candidate
    std::size_t harmonic2 = 1;
    // Set the width for the second candidate
    CandType::MsecTimeType width2(17.19 * data::milliseconds);
    // Set the significance for the second candidate
    double sigma2 = 7.0;
    // Set the candidate number for the second candidate
    std::size_t ident2 = 2;

    // Set the period for the third candidate
    CandType::MsecTimeType period3(986.17 * data::milliseconds);
    // Set the pdot for the third candidate
    CandType::SecPerSecType pdot3(1e-13);
    // Set the dispersion measure for the third candidate
    CandType::Dm dm3(192 * parsecs_per_cube_cm);
    // Set the harmonic number of the third candidate
    std::size_t harmonic3 = 1;
    // Set the width for the third candidate
    CandType::MsecTimeType width3(102.1 * data::milliseconds);
    // Set the significance for the third candidate
    double sigma3 = 11.0;
    // Set the candidate number for the third candidate
    std::size_t ident3 = 3;

    // Generate candidates with the specified parameters
    CandType candidate1(period1, pdot1, dm1, harmonic1, width1, sigma1, ident1);
    CandType candidate2(period2, pdot2, dm2, harmonic2, width2, sigma2, ident2);
    CandType candidate3(period3, pdot3, dm3, harmonic3, width3, sigma3, ident3);

    // Construct a Ccl object
    Ccl ccl;

    // Add the candidates to the Ccl
    ASSERT_NO_THROW(ccl.emplace(std::move(candidate1)));
    ASSERT_NO_THROW(ccl.emplace(std::move(candidate2)));
    ASSERT_NO_THROW(ccl.emplace(std::move(candidate3)));
    ASSERT_EQ(ccl.size(), 3);

}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
