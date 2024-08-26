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

namespace ska {
namespace cheetah {
namespace data {

template<typename SliceT>
OptimisedCandidateInterface<SliceT>::OptimisedCandidateInterface()
{
}

template<typename SliceT>
OptimisedCandidateInterface<SliceT>::OptimisedCandidateInterface(OptimisedCandidateInterface const& t)
    : SliceT(t)
{
}

template<typename SliceT>
OptimisedCandidateInterface<SliceT>::OptimisedCandidateInterface(SliceT const& t)
    : SliceT(t)
{
}

template<typename SliceT>
OptimisedCandidateInterface<SliceT>::OptimisedCandidateInterface(SliceT&& t)
    : SliceT(std::move(t))
{
}

template<typename SliceT>
OptimisedCandidateInterface<SliceT>&
    OptimisedCandidateInterface<SliceT>::operator=(OptimisedCandidateInterface const& t)
{
    static_cast<SliceT&>(*this) = static_cast<SliceT const&>(t);
    return *this;
}

template<typename SliceT>
typename OptimisedCandidateInterface<SliceT>::SubBand
    OptimisedCandidateInterface<SliceT>::subband(std::size_t subband_number)
{
    return (*this)[DimensionIndex<data::Frequency>(subband_number)];
}

template<typename SliceT>
typename OptimisedCandidateInterface<SliceT>::ConstSubBand
    OptimisedCandidateInterface<SliceT>::subband(std::size_t subband_number) const
{
    return (*this)[DimensionIndex<data::Frequency>(subband_number)];
}

template<typename SliceT>
typename OptimisedCandidateInterface<SliceT>::SubInt
    OptimisedCandidateInterface<SliceT>::subint(std::size_t subint_number)
{
    return (*this)[DimensionIndex<data::Time>(subint_number)];
}

template<typename SliceT>
typename OptimisedCandidateInterface<SliceT>::ConstSubInt
    OptimisedCandidateInterface<SliceT>::subint(std::size_t subint_number) const
{
    return (*this)[DimensionIndex<data::Time>(subint_number)];
}

template<typename SliceT>
typename OptimisedCandidateInterface<SliceT>::PhaseBin
    OptimisedCandidateInterface<SliceT>::phase_bin(std::size_t offset)
{
    return (*this)[DimensionIndex<pss::astrotypes::units::PhaseAngle>(offset)];
}

template<typename SliceT>
typename OptimisedCandidateInterface<SliceT>::ConstPhaseBin
    OptimisedCandidateInterface<SliceT>::phase_bin(std::size_t offset) const
{
    return (*this)[DimensionIndex<pss::astrotypes::units::PhaseAngle>(offset)];
}

template<typename SliceT>
std::size_t OptimisedCandidateInterface<SliceT>::number_of_subbands() const
{
    return this->template dimension<data::Frequency>();
}

template<typename SliceT>
std::size_t OptimisedCandidateInterface<SliceT>::number_of_subints() const
{
    return this->template dimension<data::Time>();
}

template<typename SliceT>
std::size_t OptimisedCandidateInterface<SliceT>::number_of_phase_bins() const
{
    return this->template dimension<pss::astrotypes::units::PhaseAngle>();
}



template<typename NumericalT, typename Alloc>
OptimisedCandidate<NumericalT, Alloc>::OptimisedCandidate()
    : BaseT(DimensionSize<data::Time>(0)
            , DimensionSize<data::Frequency>(0)
            , DimensionSize<pss::astrotypes::units::PhaseAngle>(0)
           )
      , _optimised_candidate_metadata()
{
}

template<typename NumericalT, typename Alloc>
OptimisedCandidate<NumericalT, Alloc>::OptimisedCandidate(
                                           DimensionSize<data::Time> time_size
                                           , DimensionSize<data::Frequency> freq_size
                                           , DimensionSize<pss::astrotypes::units::PhaseAngle> phase_size
                                       )
    : BaseT(time_size, freq_size, phase_size)
      , _optimised_candidate_metadata()
{
}

template<typename NumericalT, typename Alloc>
OptimisedCandidate<NumericalT, Alloc>::OptimisedCandidate(
                                           DimensionSize<data::Frequency> freq_size
                                           , DimensionSize<data::Time> time_size
                                           , DimensionSize<pss::astrotypes::units::PhaseAngle> phase_size
                                       )
    : BaseT(time_size, freq_size, phase_size)
      , _optimised_candidate_metadata()
{
}

template <typename NumericalT, typename Alloc>
typename OptimisedCandidate<NumericalT, Alloc>::CandidateType&
    OptimisedCandidate<NumericalT, Alloc>::metadata()
{
    return this->_optimised_candidate_metadata;
}

template <typename NumericalT, typename Alloc>
typename OptimisedCandidate<NumericalT, Alloc>::CandidateType
    const& OptimisedCandidate<NumericalT, Alloc>::metadata() const
{
    return this->_optimised_candidate_metadata;
}

} // namespace data
} // namespace cheetah
} // namespace ska
