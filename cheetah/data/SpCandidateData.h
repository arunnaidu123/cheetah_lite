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
#ifndef SKA_CHEETAH_DATA_SPCANDIDATEDATA_H
#define SKA_CHEETAH_DATA_SPCANDIDATEDATA_H

#include "TimeFrequency.h"
#include "ExtendedTimeFrequency.h"
#include "SpCandidate.h"
#include <pss/astrotypes/types/ExtendedTimeFrequency.h>
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief SpCandidates associated with a single data chank
 * @details
 */

template<typename TimeFrequencyT>
class SpCandidateData : public ExtendedTimeFrequency<TimeFrequencyT>
                      , public panda::DataChunk<SpCandidateData<TimeFrequencyT>>
{
        typedef ExtendedTimeFrequency<TimeFrequencyT> BaseT;

    public:
        template<typename T>
        class Candidate {
            public:
                typedef DedispersionMeasureType<T> Dm;
                typedef boost::units::quantity<MilliSeconds, double> MsecTimeType;
                typedef MsecTimeType Width;
                typedef MsecTimeType Duration;
                typedef pss::astrotypes::units::ModifiedJulianClock::time_point TimePointType;
                typedef T Sigma;
                typedef boost::units::quantity<MegaHertz, double> FrequencyType;

            public:
                Candidate();
                Candidate(Dm dm, TimePointType start_time_mjd, Width width, Duration duration, T sigma);

                /**
                 * @brief  Get the start time of the candidate
                 *
                 * @return The candidate start time in MJD
                 */
                TimePointType const & start_time() const;

                /**
                 * @brief  The duration in time of the candidate pulse
                 */
                Duration const& duration() const;

                /**
                 * @brief access a reference to dm.
                 *
                 * @details The dispersion measure
                 * is a measure on how much interstellar electrons disperse the pulsar's signal
                 * causing lower observing frequencies to arrive later than higher observing
                 * frequencies. It express electon column density so its units are
                 * pc cm-3.
                 *
                 * @return  The candidate dispersion measure in pc cm-3
                 */
                Dm const & dm() const;

                /**
                 * @brief access a reference to width.
                 *
                 * @details width is the duration pulse, expressed in milliseconds.
                 *
                 * @return The candidate pulse width in msec.
                 */
                Width const & width() const;

                /**
                 * @brief access a reference to standard deviation (sigma).
                 *
                 * @details sigma the computed significance of the candidate.
                 *          Here we assume the use of Gaussian equivalent significance
                 *          that maps p-values to the corresponding sigma (standard
                 *          deviation) that they would represent were the p-values
                 *          drawn from a normal distribution of zero mean and
                 *          unit variance.
                 *
                 * @return The candidate signal-to-noise ratio.
                 */
                Sigma const& sigma() const;

            private:
                Dm _dm;                           /**< candidate dispersion measure (pc/cm^3)  */
                TimePointType _start_time;        /**< MJD representing thw start of the pulse */
                Duration      _duration;          /**< candidate pulse duration (ms)           */
                Width         _width;             /**< candidate pulse width (ms)              */
                Sigma _sigma = 0.;                /**< candidate signficance                   */
        };

    private:
        typedef std::vector<Candidate<float>> CandidateContainer;

    public:
        typedef TimeFrequencyT TimeFrequencyType;
        typedef typename CandidateContainer::value_type CandidateType;
        typedef typename CandidateContainer::const_iterator ConstCandidateIterator;

    public:
        SpCandidateData();
        SpCandidateData(std::shared_ptr<TimeFrequencyT> const&);
        ~SpCandidateData();

        /**
         * @brief add a Candidate to the candidates list
         * @details the Candidate should be wholly included in the data
         */
        void add(CandidateType&& candidate);

        /**
         * @brief Iterator over the candidates
         */
        ConstCandidateIterator begin_candidates() const;

        /**
         * @brief Iterator marking the end of any candidates
         */
        ConstCandidateIterator end_candidates() const;

        /**
         * @brief get the correspondin numbered candidate (<= number_of_candidates())
         */
        CandidateType const& candidate(std::size_t candidate_number) const;

        /**
         * @brief the number of candidates associated with this data
         */
        std::size_t number_of_candidates() const;

    private:
        CandidateContainer _cands;
};


} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/SpCandidateData.cpp"

#endif // SKA_CHEETAH_DATA_SPCANDIDATEDATA_H
