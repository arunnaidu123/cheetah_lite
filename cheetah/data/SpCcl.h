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
#ifndef SKA_CHEETAH_DATA_SPCCL_H
#define SKA_CHEETAH_DATA_SPCCL_H

#include "cheetah/data/SpCandidate.h"
#include "cheetah/data/CandidateWindow.h"
#include "cheetah/data/VectorLike.h"
#include "cheetah/data/TimeFrequency.h"
#include "pss/astrotypes/units/TimeUnits.h"
#include "cheetah/utils/Architectures.h"
#include "panda/DataChunk.h"
#include <vector>
#include <utility>
#include <deque>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief
 *    SpCandidate list
 *
 * @details
 *      Class that  encapsulates the snippets of data corresponding to each single pulse candidate.
 *      One can also go through each candidate and the correponding TF slice using VectorLike functionalities.
 */

template<typename NumericalRep>
class SpCcl
    : public ska::panda::DataChunk<SpCcl<NumericalRep>>
    , public VectorLike<std::vector<SpCandidate<Cpu, float>>>
{
    public:
        typedef SpCandidate<Cpu, float> SpCandidateType;
        typedef SpCandidateType::Dm Dm;

    private:
        typedef std::vector<SpCandidateType> InnerType;
        typedef VectorLike<InnerType> BaseT;

    public:
        typedef TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef std::vector<std::shared_ptr<const TimeFrequencyType>> BlocksType;
        typedef typename BaseT::Iterator CandidateIterator;
        typedef typename BaseT::ConstIterator ConstCandidateIterator;

    public:
        class ConstDataIterator : public std::iterator<std::forward_iterator_tag, typename TimeFrequencyType::SliceType>
        {
            private:
                typedef typename BaseT::ConstIterator ConstCandidateIterator;
                typedef typename TimeFrequencyType::ConstIterator ConstIterator;
                typedef typename TimeFrequencyType::SliceType Slice;
                typedef typename TimeFrequencyType::ConstSliceType ConstSlice;

            public:
                typedef Slice value_type;
                typedef value_type& reference;
                typedef ConstSlice& const_reference;

            public:
                ConstDataIterator( CandidateWindow const& window
                            , ConstCandidateIterator begin
                            , ConstCandidateIterator const& end
                            , BlocksType const&
                            , utils::ModifiedJulianClock::time_point const& epoch
                            , boost::units::quantity<data::MilliSeconds,double> const& offset);

                bool operator==(ConstDataIterator const& d) const;
                bool operator!=(ConstDataIterator const& d) const;

                /**
                 * @brief: Get the start time
                 */
                utils::ModifiedJulianClock::time_point const& start_time() const;

                /**
                 * @brief returns true if the current block is the end of contiguous series
                 */
                bool end_block() const;

                /**
                 * @brief get the iterator to the first candidate of the current block
                 */
                ConstCandidateIterator candidate_begin() const;
                ConstCandidateIterator const& candidate_end() const;

                /**
                 * @brief pre-increment operator
                 */
                ConstDataIterator& operator++();

                /**
                 * @brief post-increment operator
                 */
                ConstDataIterator operator++(int);

                const_reference operator*() const;

            protected:
                void next_candidate();
                boost::units::quantity<data::MilliSeconds,double> offset() const;

            private:
                struct CandidateData {
                    friend class SpCcl<NumericalRep>;
                    CandidateData(ConstSlice const& slice, bool is_last=false)
                        : _slice(slice)
                        , _last_block(is_last) {}

                        ConstSlice& slice() { return _slice; }
                        bool last_block() const { return _last_block; };

                    private:
                        ConstSlice _slice;
                        bool _last_block; // marks the last data chunk in a group
                };

            private:
                CandidateWindow const& _window;
                ConstCandidateIterator _cand_it;
                ConstCandidateIterator const _cand_it_end;
                mutable std::deque<CandidateData> _slices;
                typename BlocksType::const_iterator _tf_it;
                typename BlocksType::const_iterator _tf_end;
                typename BlocksType::const_iterator _tf_start_it;
                utils::ModifiedJulianClock::time_point const _epoch;
                typename boost::units::quantity<MilliSeconds, double> _duration;
                typename boost::units::quantity<MilliSeconds, double> _span;
                utils::ModifiedJulianClock::time_point _start_time;
                std::deque<std::pair<ConstCandidateIterator, ConstCandidateIterator>> _cands;
        };

    public:
        SpCcl();
        SpCcl(BlocksType const&, data::DimensionIndex<data::Time> start_offset);
        SpCcl(BlocksType const&, boost::units::quantity<MilliSeconds, double> offset_time);
        ~SpCcl();

        /**
         * @brief      Access the TimeFrequency blocks associated with
         *             the single pulse candidtes.
         *
         * @return     A vector of shared_ptrs to TimeFrequency blocks
         */
        BlocksType const& tf_blocks() const;

        /**
         * @brief      Push back method for inserting Single pulse Candidates but without calculating tend
         * @details    They will be inserted in a sorted order with respect to
         *             the start time of each candidate instead of insert to the end
         */
        void push_back(SpCandidateType const& cand);

        /**
         * @brief      Push back method for inserting Single pulse Candidates calculating temd based on dispersion and freq range
         * @details    They will be inserted in a sorted order with respect to
         *             the start time of each candidate instead of insert to the end
         */
        void push_back_calculate_duration(SpCandidateType const& cand);

        /**
         * @brief      Emplace back method for inserting Single pulse Candidates, ensuring ordering by start_time.
         * @details    They will be inserted in a sorted order with respect to
         *             the start time of each candidate instead of insert to the end
         */
        void emplace(SpCandidateType && cand);
        void emplace_calculate_duration(SpCandidateType && cand);

        /**
         * @brief      Add a Single pulse Candidate to the end of the cand list, detrminig its correct position
         */
        void emplace(typename SpCandidateType::Dm,
                                   boost::units::quantity<MilliSeconds, double> start_time,
                                   boost::units::quantity<MilliSeconds, double> pulse_width,
                                   float sigma,
                                   std::size_t ident = 0
                                   );

        /**
         * @brief      Add a Single pulse Candidate to the end of the cand list, without calculating the duration
         * @details    Checking of ordering is made, and if found to be out of order will call emplace.
         */
        void emplace_back(SpCandidateType && cand);

        /**
         * @brief      Add a  Single pulse Candidate to the end of the cand list
         * @details    WARNING: No checking of ordering is made. If you use this routine you should be
         *             certain to add only pre-ordered Canadidates (ordered by start time).
         */
        void emplace_back(typename SpCandidateType::Dm,
                                   boost::units::quantity<MilliSeconds, double> start_time,
                                   boost::units::quantity<MilliSeconds, double> pulse_width,
                                   float sigma,
                                   std::size_t ident = 0
                                   );

        /**
         * @brief      Extract data corresponding to the candidates start and end times.
         * @param      CandidateWindow is the lead and post times before and after the start and end times of each condidate
         *             to include.
         * @details
         */
        ConstDataIterator data_begin(CandidateWindow const& window) const;

        ConstDataIterator data_end() const;

        /// the start time of the first relevant data (taking into account the offset)
        utils::ModifiedJulianClock::time_point const& start_time() const;

        /**
         * @brief the time duration from the beginning of the first block to start_time()
         */
        boost::units::quantity<data::MilliSeconds,double> offset_time() const;

        /**
         * @brief the max and minimum Dispersion Measure values of the candidates
         */
        std::pair<Dm, Dm> const& dm_range() const;

        /*
         * @brief the start time of the Candidate
         */
        utils::ModifiedJulianClock::time_point start_time(SpCandidateType const&) const;

        /**
         *  @brief remove candidates that meet the specified condition
         */
        template<typename PredicateT>
        void remove_if(PredicateT const&);

    protected:
        void dm_max_min(Dm);

    private:
        std::pair<Dm, Dm> _dm_range;
        BlocksType _blocks;
        utils::ModifiedJulianClock::time_point _start_time;
        boost::units::quantity<data::MilliSeconds,double> _offset_time;
        typedef decltype(1.0/(FrequencyType() * FrequencyType())) DmFactorType;
        DmFactorType _dm_delay_factor;   // to calculate candidate end time
};


} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/SpCcl.cpp"

#endif // SKA_CHEETAH_DATA_SPCCL_H
