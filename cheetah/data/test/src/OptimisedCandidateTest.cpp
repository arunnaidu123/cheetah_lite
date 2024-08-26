/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022-2024 The SKA organisation
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


#include "cheetah/data/test/OptimisedCandidateTest.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Units.h"
#include <type_traits>
#include <numeric>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template<typename TypeParam>
void OptimisedCandidateTest<TypeParam>::SetUp()
{
    // Create a candidate data cube
    _candidate = new data::OptimisedCandidate<
                               typename TypeParam::value_type
                           >
                     (
                         data::DimensionSize<data::Time>(100)
                         , data::DimensionSize<data::Frequency>(4096)
                         , data::DimensionSize<pss::astrotypes::units::PhaseAngle>(256)
                     );
}

template<typename TypeParam>
void OptimisedCandidateTest<TypeParam>::TearDown()
{
    delete _candidate;
}



//--------------------------- TEST METADATA ---------------------------//
//---------------------------------------------------------------------//

// Helper to run specific tests for non-const data types
template<typename T, typename Enable=void>
struct MetadataHelper
{
    static inline void exec(data::OptimisedCandidate<typename T::value_type>* candidate)
    {
        typename data::OptimisedCandidate<typename T::value_type>::CandidateType::MsecTimeType
            period_val(0.0138 * boost::units::si::seconds);

        typename data::OptimisedCandidate<typename T::value_type>::CandidateType::SecPerSecType
            pdot_val(0.001);

        typename data::OptimisedCandidate<typename T::value_type>::CandidateType::Dm
            dm_val(123.5 * parsecs_per_cube_cm);

        std::size_t harm_val = 1;

        typename data::OptimisedCandidate<typename T::value_type>::CandidateType::MsecTimeType
            width_val(0.1235 * boost::units::si::seconds);

        float sigma_val = 11.5;



        // Change all metadata parameters
        candidate->metadata().period(period_val);
        candidate->metadata().pdot(pdot_val);
        candidate->metadata().dm(dm_val);
        candidate->metadata().harmonic(harm_val);
        candidate->metadata().width(width_val);
        candidate->metadata().sigma(sigma_val);

        // Verify all metadata parameters match changed values
        ASSERT_EQ(candidate->metadata().period().value(), period_val.value());
        ASSERT_EQ(candidate->metadata().pdot().value(), pdot_val);
        ASSERT_EQ(candidate->metadata().dm().value(), dm_val.value());
        ASSERT_EQ(candidate->metadata().harmonic(), harm_val);
        ASSERT_EQ(candidate->metadata().width().value(), width_val.value());
        ASSERT_EQ(candidate->metadata().sigma(), sigma_val);
    }
};

// Helper to run specific tests for const data types
template<typename T>
struct MetadataHelper<T, typename std::enable_if<std::is_const<T>::value>::type>
{
    static inline void exec(const data::OptimisedCandidate<typename T::value_type>*)
    {
    }
};



// Test that metadata can be set/changed/read
TYPED_TEST(OptimisedCandidateTest, test_set_and_read_metadata)
{
    // Verify default-initialised values of metadata parameters
    ASSERT_EQ(this->_candidate->metadata().period().value(), 0);
    ASSERT_EQ(this->_candidate->metadata().pdot().value(), 0);
    ASSERT_EQ(this->_candidate->metadata().dm().value(), 0);
    ASSERT_EQ(this->_candidate->metadata().harmonic(), 0);
    ASSERT_EQ(this->_candidate->metadata().width().value(), 0);
    ASSERT_EQ(this->_candidate->metadata().sigma(), 0);

    // Call to change metadata parameters only for non-const types
    MetadataHelper<TypeParam>::exec(this->_candidate);
}



//----------------------------- TEST DATA -----------------------------//
//---------------------------------------------------------------------//

// Helper to run specific tests for non-const data types
template<typename T, typename Enable=void>
struct IteratorAssignmentHelper
{
    template<typename SliceT>
    static inline void exec(SliceT& slice)
    {
        // Create a vector with the same size as the number of elements, with
        // values incrementing up from 1
        std::vector<typename T::value_type> reference_data(slice.data_size());
        std::iota(reference_data.begin(), reference_data.end(), 1);

        // Use the iterator interface to set the elements to values
        // incrementally increasing from 1
        std::copy(reference_data.begin(), reference_data.end(), slice.begin());

        // Verify that each element has been assigned a number increasing from 1
        ASSERT_TRUE(std::equal(slice.begin(), slice.end(), reference_data.begin()));
    }
};

// Helper to run specific tests for const data types
template<typename T>
struct IteratorAssignmentHelper<T, typename std::enable_if<std::is_const<T>::value>::type>
{
    template<typename SliceT>
    static inline void exec(SliceT& slice)
    {
        // Verify that the iterator is const, even though we don't specify a const iterator
        auto it = slice.begin();
        ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*it)>::type>::value);
    }
};



// Test that the number_of_*() operators return the
// correct number of subbands, subints, and phase bins
TYPED_TEST(OptimisedCandidateTest, test_dimension_accessor_operators)
{
    ASSERT_EQ(this->_candidate->number_of_subints(),    100);
    ASSERT_EQ(this->_candidate->number_of_subbands(),   4096);
    ASSERT_EQ(this->_candidate->number_of_phase_bins(), 256);
}



//------------------------------- SUBBAND -------------------------------//

// Test that the subband() operator returns the correct number of
// subbands (should be a single SubBand), subints, and phase bins
TYPED_TEST(OptimisedCandidateTest, test_subband_operator)
{
    // Get the first subband (dimensions of phase vs time)
    auto subband = this->_candidate->subband(0);

    // Verify the dimensions of this slice are correct (i.e. only one subband)
    ASSERT_EQ(subband.number_of_subints(),    100);
    ASSERT_EQ(subband.number_of_subbands(),   1);
    ASSERT_EQ(subband.number_of_phase_bins(), 256);
}

// Test that slicing out a SubInt from a SubBand returns a 1D array of PhaseBins
TYPED_TEST(OptimisedCandidateTest, test_subband_subint_slice)
{
    // Get the first subband (dimensions of phase vs time)
    auto subband = this->_candidate->subband(0);

    // Get the first subint of the subband (dimension of phase)
    auto phase_bin_array = subband.slice(DimensionSpan<data::Time>(
                                             DimensionIndex<data::Time>(0)
                                             , DimensionSize<data::Time>(1)
                                         )
                                         , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                             subband.template dimension<pss::astrotypes::units::PhaseAngle>()
                                         )
                                        );

    // Verify the dimensions of this slice are correct (1D array of PhaseBins)
    ASSERT_EQ(phase_bin_array.number_of_subints(),    1);
    ASSERT_EQ(phase_bin_array.number_of_subbands(),   1);
    ASSERT_EQ(phase_bin_array.number_of_phase_bins(), 256);
}

// Test that we can iterate properly over PhaseBins
TYPED_TEST(OptimisedCandidateTest, test_subband_subint_iterator)
{
    // Get the first subband (dimensions of phase vs time)
    auto subband = this->_candidate->subband(0);

    // Get the first subint of the subband (dimension of phase)
    auto phase_bin_array = subband.slice(DimensionSpan<data::Time>(
                                             DimensionIndex<data::Time>(0)
                                             , DimensionSize<data::Time>(1)
                                         )
                                         , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                             subband.template dimension<pss::astrotypes::units::PhaseAngle>()
                                         )
                                        );

    // Verify that each phase bin is set to a value of zero
    for(auto it = phase_bin_array.begin(); it != phase_bin_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the PhaseBin iterator is const when we specify a const iterator
    auto const_it = phase_bin_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each phase bin is set to a value of zero
    for(; const_it != phase_bin_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run separate tests for const and non-const types
    IteratorAssignmentHelper<TypeParam>::exec(phase_bin_array);
}

// Test that slicing out a PhaseBin from a SubBand returns a 1D array of SubInts
TYPED_TEST(OptimisedCandidateTest, test_subband_phase_bin_slice)
{
    // Get the first subband (dimensions of phase vs time)
    auto subband = this->_candidate->subband(0);

    // Get the first phase bin of the subband (dimension of time)
    auto subint_array = subband.slice(DimensionSpan<data::Time>(
                                          subband.template dimension<data::Time>()
                                      )
                                      , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                          DimensionIndex<pss::astrotypes::units::PhaseAngle>(0)
                                          , DimensionSize<pss::astrotypes::units::PhaseAngle>(1)
                                      )
                                     );

    // Verify the dimensions of this slice are correct (1D array of SubInts)
    ASSERT_EQ(subint_array.number_of_subints(),    100);
    ASSERT_EQ(subint_array.number_of_subbands(),   1);
    ASSERT_EQ(subint_array.number_of_phase_bins(), 1);
}

// Test that we can iterate properly over SubInts
TYPED_TEST(OptimisedCandidateTest, test_subband_phase_bin_iterator)
{
    // Get the first subband (dimensions of phase vs time)
    auto subband = this->_candidate->subband(0);

    // Get the first phase bin of the subband (dimension of time)
    auto subint_array = subband.slice(DimensionSpan<data::Time>(
                                          subband.template dimension<data::Time>()
                                      )
                                      , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                          DimensionIndex<pss::astrotypes::units::PhaseAngle>(0)
                                          , DimensionSize<pss::astrotypes::units::PhaseAngle>(1)
                                      )
                                     );

    // Verify that each subint is set to a value of zero
    for(auto it = subint_array.begin(); it != subint_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the SubInt iterator is const when we specify a const iterator
    auto const_it = subint_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each subint is set to a value of zero
    for(; const_it != subint_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run separate tests for const and non-const types
    IteratorAssignmentHelper<TypeParam>::exec(subint_array);
}



//------------------------------- SUBINT -------------------------------//

// Test that the subint() operator returns the correct number of
// subbands, subints (should be a single SubInt), and phase bins
TYPED_TEST(OptimisedCandidateTest, test_subint_operator)
{
    // Get the first subint (dimensions of phase vs frequency)
    auto subint = this->_candidate->subint(0);

    // Verify the dimensions of this slice are correct (i.e. only one subint)
    ASSERT_EQ(subint.number_of_subints(),    1);
    ASSERT_EQ(subint.number_of_subbands(),   4096);
    ASSERT_EQ(subint.number_of_phase_bins(), 256);
}

// Test that slicing out a SubBand from a SubInt returns a 1D array of PhaseBins
TYPED_TEST(OptimisedCandidateTest, test_subint_subband_slice)
{
    // Get the first subint (dimensions of phase vs frequency)
    auto subint = this->_candidate->subint(0);

    // Get the first subband of the subint (dimension of phase)
    auto phase_bin_array = subint.slice(DimensionSpan<data::Frequency>(
                                            DimensionIndex<data::Frequency>(0)
                                            , DimensionSize<data::Frequency>(1)
                                        )
                                        , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                            subint.template dimension<pss::astrotypes::units::PhaseAngle>()
                                        )
                                       );

    // Verify the dimensions of this slice are correct (1D array of PhaseBins)
    ASSERT_EQ(phase_bin_array.number_of_subints(),    1);
    ASSERT_EQ(phase_bin_array.number_of_subbands(),   1);
    ASSERT_EQ(phase_bin_array.number_of_phase_bins(), 256);
}

// Test that we can iterate properly over PhaseBins
TYPED_TEST(OptimisedCandidateTest, test_subint_subband_iterator)
{
    // Get the first subint (dimensions of phase vs frequency)
    auto subint = this->_candidate->subint(0);

    // Get the first subband of the subint (dimension of phase)
    auto phase_bin_array = subint.slice(DimensionSpan<data::Frequency>(
                                            DimensionIndex<data::Frequency>(0)
                                            , DimensionSize<data::Frequency>(1)
                                        )
                                        , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                            subint.template dimension<pss::astrotypes::units::PhaseAngle>()
                                        )
                                       );

    // Verify that each phase bin is set to a value of zero
    for(auto it = phase_bin_array.begin(); it != phase_bin_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the PhaseBin iterator is const when we specify a const iterator
    auto const_it = phase_bin_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each phase bin is set to a value of zero
    for(; const_it != phase_bin_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run separate tests for const and non-const types
    IteratorAssignmentHelper<TypeParam>::exec(phase_bin_array);
}

// Test that slicing out a PhaseBin from a SubInt returns a 1D array of SubBands
TYPED_TEST(OptimisedCandidateTest, test_subint_phase_bin_slice)
{
    // Get the first subint (dimensions of phase vs frequency)
    auto subint = this->_candidate->subint(0);

    // Get the first phase bin of the subint (dimension of frequency)
    auto subband_array = subint.slice(DimensionSpan<data::Frequency>(
                                          subint.template dimension<data::Frequency>()
                                      )
                                      , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                          DimensionIndex<pss::astrotypes::units::PhaseAngle>(0)
                                          , DimensionSize<pss::astrotypes::units::PhaseAngle>(1)
                                      )
                                     );

    // Verify the dimensions of this slice are correct (1D array of SubBands)
    ASSERT_EQ(subband_array.number_of_subints(),    1);
    ASSERT_EQ(subband_array.number_of_subbands(),   4096);
    ASSERT_EQ(subband_array.number_of_phase_bins(), 1);
}

// Test that we can iterate properly over SubBands
TYPED_TEST(OptimisedCandidateTest, test_subint_phase_bin_iterator)
{
    // Get the first subint (dimensions of phase vs frequency)
    auto subint = this->_candidate->subint(0);

    // Get the first phase bin of the subint (dimension of frequency)
    auto subband_array = subint.slice(DimensionSpan<data::Frequency>(
                                          subint.template dimension<data::Frequency>()
                                      )
                                      , DimensionSpan<pss::astrotypes::units::PhaseAngle>(
                                          DimensionIndex<pss::astrotypes::units::PhaseAngle>(0)
                                          , DimensionSize<pss::astrotypes::units::PhaseAngle>(1)
                                      )
                                     );

    // Verify that each subband is set to a value of zero
    for(auto it = subband_array.begin(); it != subband_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the SubBand iterator is const when we specify a const iterator
    auto const_it = subband_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each subband is set to a value of zero
    for(; const_it != subband_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run separate tests for const and non-const types
    IteratorAssignmentHelper<TypeParam>::exec(subband_array);
}



//------------------------------- PHASE -------------------------------//

// Test that the phase_bin() operator returns the correct number of
// subbands, subints, and phase bins (should be a single PhaseBin)
TYPED_TEST(OptimisedCandidateTest, test_phase_operator)
{
    // Get the first phase bin (dimensions of frequency vs time)
    auto phase_bin = this->_candidate->phase_bin(0);

    // Verify the dimensions of this slice are correct (i.e. only one subband)
    ASSERT_EQ(phase_bin.number_of_subints(),    100);
    ASSERT_EQ(phase_bin.number_of_subbands(),   4096);
    ASSERT_EQ(phase_bin.number_of_phase_bins(), 1);
}

// Test that slicing out a SubBand from a PhaseBin returns a 1D array of SubInts
TYPED_TEST(OptimisedCandidateTest, test_phase_subband_slice)
{
    // Get the first phase bin (dimensions of frequency vs time)
    auto phase_bin = this->_candidate->phase_bin(0);

    // Get the first subband of the phase bin (dimension of time)
    auto subint_array = phase_bin.slice(DimensionSpan<data::Time>(
                                            phase_bin.template dimension<data::Time>()
                                        )
                                        , DimensionSpan<data::Frequency>(
                                            DimensionIndex<data::Frequency>(0)
                                            , DimensionSize<data::Frequency>(1)
                                        )
                                       );

    // Verify the dimensions of this slice are correct (1D array of SubInts)
    ASSERT_EQ(subint_array.number_of_subints(),    100);
    ASSERT_EQ(subint_array.number_of_subbands(),   1);
    ASSERT_EQ(subint_array.number_of_phase_bins(), 1);
}

// Test that we can iterate properly over SubInts
TYPED_TEST(OptimisedCandidateTest, test_phase_bin_subband_iterator)
{
    // Get the first phase bin (dimensions of frequency vs time)
    auto phase_bin = this->_candidate->phase_bin(0);

    // Get the first subband of the phase bin (dimension of time)
    auto subint_array = phase_bin.slice(DimensionSpan<data::Time>(
                                            phase_bin.template dimension<data::Time>()
                                        )
                                        , DimensionSpan<data::Frequency>(
                                            DimensionIndex<data::Frequency>(0)
                                            , DimensionSize<data::Frequency>(1)
                                        )
                                       );

    // Verify that each subint is set to a value of zero
    for(auto it = subint_array.begin(); it != subint_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the SubInt iterator is const when we specify a const iterator
    auto const_it = subint_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each subint is set to a value of zero
    for(; const_it != subint_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run separate tests for const and non-const types
    IteratorAssignmentHelper<TypeParam>::exec(subint_array);
}

// Test that slicing out a subint() from a PhaseBin returns a 1D array of SubBands
TYPED_TEST(OptimisedCandidateTest, test_phase_bin_subint_slice)
{
    // Get the first phase bin (dimensions of frequency vs time)
    auto phase_bin = this->_candidate->phase_bin(0);

    // Get the first subint of the phase bin (dimension of frequency)
    auto subband_array = phase_bin.slice(DimensionSpan<data::Time>(
                                             DimensionIndex<data::Time>(0)
                                             , DimensionSize<data::Time>(1)
                                         )
                                         , DimensionSpan<data::Frequency>(
                                             phase_bin.template dimension<data::Frequency>()
                                         )
                                        );

    // Verify the dimensions of this slice are correct (1D array of SubBands)
    ASSERT_EQ(subband_array.number_of_subints(),    1);
    ASSERT_EQ(subband_array.number_of_subbands(),   4096);
    ASSERT_EQ(subband_array.number_of_phase_bins(), 1);
}

// Test that we can iterate properly over SubBands
TYPED_TEST(OptimisedCandidateTest, test_phase_bin_subint_iterator)
{
    // Get the first phase bin (dimensions of frequency vs time)
    auto phase_bin = this->_candidate->phase_bin(0);

    // Get the first subint of the phase bin (dimension of frequency)
    auto subband_array = phase_bin.slice(DimensionSpan<data::Time>(
                                             DimensionIndex<data::Time>(0)
                                             , DimensionSize<data::Time>(1)
                                         )
                                         , DimensionSpan<data::Frequency>(
                                             phase_bin.template dimension<data::Frequency>()
                                         )
                                        );

    // Verify that each subband is set to a value of zero
    for(auto it = subband_array.begin(); it != subband_array.end(); ++it)
    {
        ASSERT_EQ(*it, 0);
    }

    // Verify that the SubBand iterator is const when we specify a const iterator
    auto const_it = subband_array.cbegin();
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*const_it)>::type>::value);

    // Verify that each subband is set to a value of zero
    for(; const_it != subband_array.cend(); ++const_it)
    {
        ASSERT_EQ(*const_it, 0);
    }

    // Call to run separate tests for const and non-const types
    IteratorAssignmentHelper<TypeParam>::exec(subband_array);
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
