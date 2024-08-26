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
#ifndef SKA_CHEETAH_DATA_SPCANDIDATE_H
#define SKA_CHEETAH_DATA_SPCANDIDATE_H

#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/utils/Architectures.h"
#include "panda/DataChunk.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/make_scaled_unit.hpp>
#pragma GCC diagnostic pop
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A simple record to hold 'candidate' proprerties
 *
 * @details A collection of values which hold the 'candidate' proprerties
 * to be used in more complex vector-type classes
 *
 * @tparam T the underlying data representation (double, float uin8_t, etc) for the Dedispersion measure, sigma and pulse_pdot.
 *
 * Most data are double type.
 */

template <typename Arch, typename T>
class SpCandidate : public ska::panda::DataChunk<SpCandidate<Arch, T>>
{
    public:
        typedef T NumericalRep;
        typedef DedispersionMeasureType<NumericalRep> Dm;
        typedef boost::units::quantity<MilliSeconds, double> MsecTimeType;
        typedef MsecTimeType Width;
        typedef pss::astrotypes::units::ModifiedJulianClock::time_point TimePointType;
        typedef T SigmaType;
        typedef boost::units::quantity<MegaHertz, double> FrequencyType;

    public:
        /**
         * @brief Create a zero-sized SpCandidate
         */
        SpCandidate();

        /**
         * @brief   Construct a defined size SpCandidate object. tend is set to tstart
         * @param dm           = estimated/computed dispersion measure
         * @param tstart       = relative time since start of block of data searched (TODO: Decide whether this should be a time point type)
         * @param width        = estimated/computed pulse width of the candidate
         * @param sigma        = estimated significance of the candidate (is this significance or S/N)
         * @param ident        = candidate identifier, to be held constant along pipeline (debug tracing)
         */
        SpCandidate(Dm dm, MsecTimeType tstart
                         , MsecTimeType width, T sigma
                         , std::size_t ident = 0 );

        /**
         * @brief   Construct a defined size SpCandidate object. tend is set to tstart
         * @param dm           = estimated/computed dispersion measure
         * @param tstart       = relative time since start of block of data searched (TODO: Decide whether this should be a time point type)
         * @param width        = estimated/computed pulse width of the candidate
         * @param tend         = relative time since start of block of data searched (end of pulse)
         * @param sigma        = estimated significance of the candidate (is this significance or S/N)
         * @param ident        = candidate identifier, to be held constant along pipeline (debug tracing)
         */
        SpCandidate(Dm dm, MsecTimeType tstart
                         , MsecTimeType width
                         , MsecTimeType tend
                         , T sigma
                         , std::size_t ident = 0 );
        /**
         * @brief Constructor to set all params calculating the end time from the dm and frequency values supplied
         * @param dm           = estimated/computed dispersion measure
         * @param tstart       = relative time since start of block of data searched (TODO: Decide whether this should be a time point type)
         * @param width        = estimated/computed pulse width of the candidate
         * @param sigma        = estimated significance of the candidate (is this significance or S/N)
         * @param low         = lowest frequency channel
         * @param high        = highest frequency channel
         */
        SpCandidate(Dm dm, MsecTimeType tstart
                         , MsecTimeType width
                         , T sigma
                         , FrequencyType low
                         , FrequencyType high
                         , std::size_t ident = 0 );

        /**
         * @brief destroy a  SpCandidate
         */
        ~SpCandidate();

        /**
         * @brief  Get the start time of the candidate
         *
         * @return The candidate start time in milliseconds since the start of the searched block.
         */
        MsecTimeType const & tstart() const;

        /**
         * @brief Set the candidate tstart
         *
         * @param [in]  c_tstart    the candidate start time in milliseconds since the start of the searched block.
         */
        void tstart(MsecTimeType c_tstart);

        /**
         * @brief Get the end time
         *
         * @return The candidate end time in milliseconds since the start of the searched block
         */
        MsecTimeType const& tend() const;
        void tend(MsecTimeType);

        /**
         * @brief Set the end time
         * @details The end time is calculated based on the DM of the candidate
         *         and the start and end frequency of the TF block
         */
        void duration_end(MsecTimeType c_tend);

        /**
         * @brief Set the end time
         * @details  This step is important as the DM will be set along with the end time
         */
        void dm_with_duration_end(FrequencyType const& f_high, FrequencyType const& f_low, Dm const& dm );

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
        MsecTimeType const & width() const;

        /**
         * @brief set the candidate width value.
         *
         * @details width is the duration pulse, expressed in milliseconds.
         *
         * @param [in] c_width  the candidate pulse width in msec.
         */
        void width(MsecTimeType c_width) ;

        /**
         * @brief access a reference to _sigma.
         *
         * @details _sigma is the computed significance of the candidate.
         *          Here we assume the use of Gaussian equivalent significance
         *          that maps p-values to the corresponding sigma (standard
         *          deviation) that they would represent were the p-values
         *          drawn from a normal distribution of zero mean and
         *          unit variance.
         *
         * @return The candidate signal-to-noise ratio.
         */
        T const & sigma() const;

        /**
         * @brief Set the candidate _sigma value.
         *
         * @details _sigma is the computed significance of the candidate.
         *          Here we assume the use of Gaussian equivalent significance
         *          that maps p-values to the corresponding sigma (standard
         *          deviation) that they would represent were the p-values
         *          drawn from a normal distribution of zero mean and
         *          unit variance.
         *
         * @param [in] c_sigma     the candidate signal-to-noise ratio
         */
        void sigma(T c_sigma) ;

        /**
         * @brief access a reference to ident.
         *
         * @details ident is an optional
         * identifier label to let follow the trail of a specific candidate
         * across different modules.
         *
         * @return The candidate ID number.
         */
        std::size_t const & ident() const;

        /**
         * @brief set the candidate ident value.
         *
         * @details ident is an optional
         * identifier label to let follow the trail of a specific candidate
         * across different modules.
         *
         * @param [in] c_ident      the candidate ID number
         */
        void ident(std::size_t c_ident) ;

    private:
        Dm _dm;                           /**< candidate dispersion measure (pc/cm^3) */
        MsecTimeType  _tstart;            /**< candidate start time (ms)             */
        MsecTimeType  _tend;              /**< candidate end time (ms)             */
        MsecTimeType  _width;             /**< candidate pulse width (ms)             */
        T _sigma = 0.;                    /**< candidate signficance                  */
        std::size_t _ident;               /**< candidate ID                           */
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/SpCandidate.cpp"

#endif // SKA_CHEETAH_DATA_SPCANDIDATE_H
