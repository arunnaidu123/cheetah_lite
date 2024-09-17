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
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/VectorLike.h"
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
    //: public ska::panda::DataChunk<SpCcl<NumericalRep>>
{
    public:
        typedef SpCandidate<Cpu, float> SpCandidateType;
        typedef SpCandidateType::Dm Dm;
        typedef data::DmTrials<cheetah::Cpu, float> DmTrialsType;
    //private:
    //    typedef std::vector<SpCandidateType> InnerType;
    //    typedef VectorLike<InnerType> BaseT;

    public:
        SpCcl();
        SpCcl(std::shared_ptr<DmTrialsType> const& data);
        ~SpCcl();

        /**
         * @brief      Push back method for inserting Single pulse Candidates but without calculating tend
         * @details    They will be inserted in a sorted order with respect to
         *             the start time of each candidate instead of insert to the end
         */
        void push_back(SpCandidateType const& cand);

        /**
         * @brief      Emplace back method for inserting Single pulse Candidates, ensuring ordering by start_time.
         * @details    They will be inserted in a sorted order with respect to
         *             the start time of each candidate instead of insert to the end
         */
        void emplace(SpCandidateType && cand);

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

        std::size_t size() const;

    protected:
        void dm_max_min(Dm);

    private:
        std::pair<Dm, Dm> _dm_range;
        utils::ModifiedJulianClock::time_point _start_time;
        boost::units::quantity<data::MilliSeconds,double> _offset_time;
        std::vector<SpCandidateType> _data;
};


} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/SpCcl.cpp"

#endif // SKA_CHEETAH_DATA_SPCCL_H
