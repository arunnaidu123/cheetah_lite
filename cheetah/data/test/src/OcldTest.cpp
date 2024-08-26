/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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

#include "cheetah/data/test/OcldTest.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template<typename TypeParam>
OcldTest<TypeParam>::OcldTest()
    : ::testing::Test()
{
}

template<typename TypeParam>
OcldTest<TypeParam>::~OcldTest()
{
}

template<typename TypeParam>
void OcldTest<TypeParam>::SetUp()
{
    // Create a candidate data cube
    _candidate = new data::OptimisedCandidate<TypeParam>(data::DimensionSize<data::Time>(100),
                                                         data::DimensionSize<data::Frequency>(4096),
                                                         data::DimensionSize<pss::astrotypes::units::PhaseAngle>(256));

    // Set metadata parameters
    typename data::Ocld<TypeParam>::BaseT::ValueType::CandidateType::MsecTimeType period_val(0.00123 * boost::units::si::seconds);
    typename data::Ocld<TypeParam>::BaseT::ValueType::CandidateType::SecPerSecType pdot_val(0.00001);
    typename data::Ocld<TypeParam>::BaseT::ValueType::CandidateType::Dm dm_val(123.5 * parsecs_per_cube_cm);
    typename data::Ocld<TypeParam>::BaseT::ValueType::CandidateType::MsecTimeType width_val(0.1235 * boost::units::si::seconds);
    float sigma_val = 11.5;

    // Initialise metadata parameters in candidate
    _candidate->metadata().period(period_val);
    _candidate->metadata().pdot(pdot_val);
    _candidate->metadata().dm(dm_val);
    _candidate->metadata().width(width_val);
    _candidate->metadata().sigma(sigma_val);
}

template<typename TypeParam>
void OcldTest<TypeParam>::TearDown()
{
    delete _candidate;
}

// Test adding candidates to and iterating over candidates in an Ocld
TYPED_TEST(OcldTest, test_candidates)
{
    // Create an empty Ocld
    data::Ocld<TypeParam> ocld;

    // Add an OptimisedCandidate to the Ocld
    ocld.add(*(this->_candidate));

    // Verify that we have one candidate in the Ocld
    ASSERT_EQ(1U, ocld.size());

    // Change some metadata parameters
    typename data::Ocld<TypeParam>::BaseT::ValueType::CandidateType::MsecTimeType period_val(0.451 * boost::units::si::seconds);
    this->_candidate->metadata().period(period_val);
    typename data::Ocld<TypeParam>::BaseT::ValueType::CandidateType::Dm dm_val(65.2 * parsec_per_cube_cm);
    this->_candidate->metadata().dm(dm_val);

    // Add the altered candidate
    ocld.add(*(this->_candidate));

    // Verify that we have two candidates in the Ocld
    ASSERT_EQ(2U, ocld.size());

    // Step through vector (forward) and verify candidates are in the correct order
    auto it_begin = ocld.begin();
    ASSERT_FLOAT_EQ(it_begin->metadata().period().value(), 1.23);
    ASSERT_FLOAT_EQ(it_begin->metadata().dm().value(), 123.5);
    ASSERT_NO_THROW(++it_begin);
    ASSERT_FLOAT_EQ(it_begin->metadata().period().value(), 451);
    ASSERT_FLOAT_EQ(it_begin->metadata().dm().value(), 65.2);
    ASSERT_NO_THROW(++it_begin);
    ASSERT_EQ(it_begin, ocld.end());

    // Step through vector (backwards) and verify candidates are in the correct order
    auto it_end = ocld.end();
    ASSERT_NO_THROW(--it_end);
    ASSERT_FLOAT_EQ(it_end->metadata().period().value(), 451);
    ASSERT_FLOAT_EQ(it_end->metadata().dm().value(), 65.2);
    ASSERT_NO_THROW(--it_end);
    ASSERT_EQ(it_end, ocld.begin());
    ASSERT_FLOAT_EQ(it_end->metadata().period().value(), 1.23);
    ASSERT_FLOAT_EQ(it_end->metadata().dm().value(), 123.5);

    // Test forward iteration
    it_begin = ocld.begin();
    it_end = ocld.end();
    while(it_begin != it_end)
    {
        ASSERT_FLOAT_EQ(this->_candidate->metadata().pdot().value(), 0.00001);
        ASSERT_EQ(this->_candidate->metadata().width().value(), 123.5);
        ASSERT_EQ(this->_candidate->metadata().sigma(), 11.5);
        ASSERT_NO_THROW(++it_begin);
    }

    // Test backward iteration
    it_begin = ocld.begin();
    while(it_end != it_begin)
    {
        ASSERT_FLOAT_EQ(this->_candidate->metadata().pdot().value(), 0.00001);
        ASSERT_EQ(this->_candidate->metadata().width().value(), 123.5);
        ASSERT_EQ(this->_candidate->metadata().sigma(), 11.5);
        ASSERT_NO_THROW(--it_end);
    }

    // Test forward iteration (const)
    auto it_begin_const = ocld.cbegin();
    auto it_end_const = ocld.cend();
    while(it_begin_const != it_end_const)
    {
        ASSERT_FLOAT_EQ(this->_candidate->metadata().pdot().value(), 0.00001);
        ASSERT_EQ(this->_candidate->metadata().width().value(), 123.5);
        ASSERT_EQ(this->_candidate->metadata().sigma(), 11.5);
        ASSERT_NO_THROW(++it_begin_const);
    }

    // Test backward iteration (const)
    it_begin_const = ocld.cbegin();
    while(it_end_const != it_begin_const)
    {
        ASSERT_FLOAT_EQ(this->_candidate->metadata().pdot().value(), 0.00001);
        ASSERT_EQ(this->_candidate->metadata().width().value(), 123.5);
        ASSERT_EQ(this->_candidate->metadata().sigma(), 11.5);
        ASSERT_NO_THROW(--it_end_const);
    }
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
