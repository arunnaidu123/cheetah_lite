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
#ifndef SKA_CHEETAH_DATA_OPTIMISEDCANDIDATE_H
#define SKA_CHEETAH_DATA_OPTIMISEDCANDIDATE_H

#include "pss/astrotypes/multiarray/MultiArray.h"
#include "pss/astrotypes/units/Phase.h"
#include "cheetah/data/Candidate.h"
#include "cheetah/data/Units.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @class OptimisedCandidate
 * @brief
 *    An optimised candidate and its corresponding data cube
 *
 * @details Holds a folded data cube output by the FLDO module with
 *          dimensions of (Time x Frequency x Phase), as well as a list
 *          of the parameters used by FLDO to generate the data cube.
 *
 */

template<typename SliceT>
class OptimisedCandidateInterface : public SliceT
{
    protected:
        typedef typename SliceT::SliceType SliceType;

    public:
        typedef typename SliceType::template OperatorSliceType<data::Frequency>::type SubBand;
        typedef typename SliceType::template ConstOperatorSliceType<data::Frequency>::type ConstSubBand;
        typedef typename SliceType::template OperatorSliceType<data::Time>::type SubInt;
        typedef typename SliceType::template ConstOperatorSliceType<data::Time>::type ConstSubInt;
        typedef typename SliceType::template OperatorSliceType<pss::astrotypes::units::PhaseAngle>::type PhaseBin;
        typedef typename SliceType::template ConstOperatorSliceType<pss::astrotypes::units::PhaseAngle>::type ConstPhaseBin;


    public:
        // expose SliceT constructors
        using SliceT::SliceT;

    public:
        OptimisedCandidateInterface();
        OptimisedCandidateInterface(OptimisedCandidateInterface const&);
        OptimisedCandidateInterface(SliceT const& t);
        OptimisedCandidateInterface(SliceT&& t);

        OptimisedCandidateInterface& operator=(OptimisedCandidateInterface const&);

        /** @brief   Return a single frequency subband across all time subintegrations and all phase bins
         *
         *  @details The type returned is a MultiArray @class Slice
         *           with all the features of that available
         *
         *  @example
         *  @code
         *  // Create a candidate data cube
         *  OptimisedCandidate<uint16_t> candidate(DimensionSize<Time>(100)
         *                                         , DimensionSize<Frequency>(4096)
         *                                         , DimensionSize<Phase>(256)
         *                                        );
         *
         *  // Get the first subband
         *  typename OptimisedCandidate<uint16_t>::SubBand subband = candidate.subband(0);
         *  @endcode
         */
        SubBand subband(std::size_t subband_number);
        ConstSubBand subband(std::size_t subband_number) const;

        /** @brief   Return a single time subintegration across all frequency subbands and all phase bins
         *
         *  @details The type returned is a MultiArray @class Slice
         *           with all the features of that available
         *
         *  @example
         *  @code
         *  // Create a candidate data cube
         *  OptimisedCandidate<uint16_t> candidate(DimensionSize<Time>(100)
         *                                         , DimensionSize<Frequency>(4096)
         *                                         , DimensionSize<Phase>(256)
         *                                        );
         *
         *  // Get the first subintegration
         *  typename OptimisedCandidate<uint16_t>::SubInt subint = candidate.subint(0);
         *  @endcode
         */
        SubInt subint(std::size_t subint_number);
        ConstSubInt subint(std::size_t subint_number) const;

        /** @brief   Return a single phase bin across all frequency subbands
         *           and all time subintegrations from the specified offset
         *
         *  @details The type returned is a MultiArray @class Slice
         *           with all the features of that available
         *
         *  @example
         *  @code
         *  // Create a candidate data cube
         *  OptimisedCandidate<uint16_t> candidate(DimensionSize<Time>(100)
         *                                         , DimensionSize<Frequency>(4096)
         *                                         , DimensionSize<Phase>(256)
         *                                        );
         *
         *  // Get the first phase bin
         *  typename OptimisedCandidate<uint16_t>::PhaseBin phase_bin = candidate.phase_bin(0);
         *  @endcode
         */
        PhaseBin phase_bin(std::size_t offset);
        ConstPhaseBin phase_bin(std::size_t offset) const;

        /** @brief   Return the number of subbands in the data structure
         *  @details A synonym for dimension<Frequency>()
         */
        std::size_t number_of_subbands() const;

        /** @brief   Return the number of subintegrations in the data structure
         *  @details A synonym for dimension<Time>()
         */
        std::size_t number_of_subints() const;

        /** @brief   Return the number of phase bins in the data structure
         *  @details A synonym for dimension<Phase>()
         */
        std::size_t number_of_phase_bins() const;

};

template<typename NumericalT, typename Alloc=std::allocator<NumericalT>>
class OptimisedCandidate : public OptimisedCandidateInterface<
                                      pss::astrotypes::multiarray::MultiArray<
                                          Alloc
                                          , NumericalT
                                          , OptimisedCandidateInterface
                                          , data::Time
                                          , data::Frequency
                                          , pss::astrotypes::units::PhaseAngle
                                      >
                                  >
{

    private:
        typedef OptimisedCandidateInterface<
                    pss::astrotypes::multiarray::MultiArray<
                        Alloc
                        , NumericalT
                        , OptimisedCandidateInterface
                        , data::Time
                        , data::Frequency
                        , pss::astrotypes::units::PhaseAngle
                    >
                >
                BaseT;

    public:
        typedef typename BaseT::SubBand SubBand;
        typedef typename BaseT::ConstSubBand ConstSubBand;
        typedef typename BaseT::SubInt SubInt;
        typedef typename BaseT::ConstSubInt ConstSubInt;
        typedef typename BaseT::PhaseBin PhaseBin;
        typedef typename BaseT::ConstPhaseBin ConstPhaseBin;

        typedef typename BaseT::SliceType SliceType;

        typedef NumericalT value_type;
        typedef Candidate<Cpu, float> CandidateType;

    public:
        OptimisedCandidate();
        OptimisedCandidate(DimensionSize<data::Time>
                           , DimensionSize<data::Frequency>
                           , DimensionSize<pss::astrotypes::units::PhaseAngle>
                          );
        OptimisedCandidate(DimensionSize<data::Frequency>
                           , DimensionSize<data::Time>
                           , DimensionSize<pss::astrotypes::units::PhaseAngle>
                          );

        ~OptimisedCandidate() = default;

    public:
        /** @brief Get candidate metadata
         */
        CandidateType& metadata();
        CandidateType const& metadata() const;

    private:
        CandidateType _optimised_candidate_metadata;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "detail/OptimisedCandidate.cpp"

#endif // SKA_CHEETAH_DATA_OPTIMISEDCANDIDATE_H
