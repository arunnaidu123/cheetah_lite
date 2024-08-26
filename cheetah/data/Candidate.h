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
#ifndef SKA_CHEETAH_DATA_CANDIDATE_H
#define SKA_CHEETAH_DATA_CANDIDATE_H

#include <vector>
#include "panda/DataChunk.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/utils/Architectures.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/make_scaled_unit.hpp>
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A simple record to hold 'candidate' properties
 *
 * @details A collection of values which hold the 'candidate' properties
 *          to be used in more complex vector-type classes
 *
 * @tparam T   The underlying data representation (double, float uin8_t, etc)
 *             for the dispersion measure, sigma and pulse_pdot.
 *
 * Most data are double type.
 *
 *
 */

template <typename Arch, typename T>
class Candidate : public ska::panda::DataChunk<Candidate<Arch, T>>
{
    public:
        typedef DedispersionMeasureType<T> Dm;
        typedef boost::units::quantity<MilliSeconds, double> MsecTimeType;
        typedef MsecTimeType TimeType;
        typedef boost::units::quantity<boost::units::si::dimensionless, T> SecPerSecType;

    public:
        /**
         * @brief Create a zero-sized Candidate
         */
        Candidate();

        /**
         * @brief   Construct a defined size Candidate object
         * @param pulse_period  = estimated/computed pulse period
         * @param pulse_pdot    = estimated/computed period acceleration
         * @param dm            = estimated/computed dispersion measure
         * @param ident         = candidate identifier, to be held constant along pipeline (debug tracing)
         */
        Candidate(MsecTimeType pulse_period
                  , SecPerSecType pulse_pdot
                  , Dm dm
                  , std::size_t ident = 0
                 );

        /**
         * @brief   Construct a defined size Candidate object
         * @param pulse_period  = estimated/computed pulse period
         * @param pulse_pdot    = estimated/computed period acceleration
         * @param dm            = estimated/computed dispersion measure
         * @param harm_num      = harmonic number at which this candidate was detected
         * @param pulse_width   = computed pulse width
         * @param sigma         = computed significance
         * @param ident         = candidate identifier, to be held constant along pipeline (debug tracing)
         */
        Candidate(MsecTimeType pulse_period
                  , SecPerSecType pulse_pdot
                  , Dm dm
                  , std::size_t harm_num
                  , MsecTimeType pulse_width
                  , T sigma
                  , std::size_t ident = 0
                 );

        /**
         * @brief destroy a Candidate
         */
        ~Candidate();

        /**
         * @brief access a reference to candidate period.
         *
         * @return The candidate period in milliseconds
         */
        MsecTimeType const& period() const;

        /**
         * @brief Set the candidate period.
         *
         * @param [in]  c_period   The candidate period in milliseconds
         */
        void period(MsecTimeType c_period);

        /**
         * @brief access a reference to pulse_pdot.
         *
         * @details The pulse_pdot, or Period Derivative, is the first time
         *          derivative of the period. It has units of seconds per
         *          second. This is intrinsic to all pulsars, but could also
         *          have a component induced by a companion star or planets
         *          orbiting the pulsar.
         *
         * @return The period derivative value in units of sec/sec
         */
        SecPerSecType const& pdot() const;

        /**
         * @brief Set the candidate pulse_pdot value.
         *
         * @details The pulse_pdot, or Period Derivative, is the first time
         *          derivative of the period. It has units of seconds per
         *          second. This is intrinsic to all pulsars, but could also
         *          have a component induced by a companion star or planets
         *          orbiting the pulsar.
         *
         * @param [in] c_pulse_pdot   The candidate period derivative in sec/sec
         */
        void pdot(SecPerSecType c_pulse_pdot);

        /**
         * @brief access a reference to dm.
         *
         * @details The dispersion measure (DM) is a measure on how much
         *          interstellar electrons disperse the pulsar's signal
         *          causing lower observing frequencies to arrive later
         *          than higher observing frequencies. It is expressed as
         *          an electon column density, so its units are pc cm-3.
         *
         * @return The candidate dispersion measure in pc cm-3
         */
        Dm const& dm() const;

        /**
         * @brief Set the candidate dm value.
         *
         * @details The dispersion measure (DM) is a measure on how much
         *          interstellar electrons disperse the pulsar's signal
         *          causing lower observing frequencies to arrive later
         *          than higher observing frequencies. It is expressed as
         *          an electon column density, so its units are pc cm-3.
         *
         * @param [in] dm   The candidate dispersion measure
         */
        void dm(Dm c_dm);

        /**
         * @brief access a reference to harmonic number.
         *
         * @details the harmonic number is the number of frequency harmonics
         *          that have been added together for a reported candidate.
         *
         * @return The harmonic number at which a candidate was detected
         */
        std::size_t const& harmonic() const;

        /**
         * @brief set the candidate harmonic number.
         *
         * @details the harmonic number is the number of frequency harmonics
         *          that have been added together for a reported candidate.
         *
         * @param [in] c_harm_num   The candidate harmonic number
         */
        void harmonic(std::size_t c_harm_num);

        /**
         * @brief access a reference to width.
         *
         * @details width is the length of the pulses as computed by the FLDO
         *          module. It is expressed in milliseconds.
         *
         * @return The candidate pulse width in milliseconds
         */
        MsecTimeType const& width() const;

        /**
         * @brief set the candidate width value.
         *
         * @details width is the length of the pulses as computed by the FLDO
         *          module. It is expressed in milliseconds.
         *
         * @param [in] c_width   The candidate pulse width in milliseconds
         */
        void width(MsecTimeType c_width);

        /**
         * @brief access a reference to _sigma.
         *
         * @details sigma is the computed significance of the candidate.
         *          Here we assume the use of Gaussian equivalent significance
         *          that maps p-values to the corresponding sigma (standard
         *          deviation) that they would represent were the p-values
         *          drawn from a normal distribution of zero mean and
         *          unit variance.
         *
         * @return The candidate signal-to-noise ratio
         */
        T const& sigma() const;

        /**
         * @brief Set the candidate sigma value.
         *
         * @details sigma is the computed significance of the candidate.
         *          Here we assume the use of Gaussian equivalent significance
         *          that maps p-values to the corresponding sigma (standard
         *          deviation) that they would represent were the p-values
         *          drawn from a normal distribution of zero mean and
         *          unit variance.
         *
         * @param [in] c_sigma   The candidate signal-to-noise ratio
         */
        void sigma(T c_sigma);

        /**
         * @brief access a reference to ident.
         *
         * @details ident is an optional identifier label to let follow the
         *          trail of a specific candidate across different modules.
         *
         * @return The candidate ID number
         */
        std::size_t const& ident() const;

        /**
         * @brief set the candidate ident value.
         *
         * @details ident is an optional identifier label to let follow the
         *          trail of a specific candidate across different modules.
         *
         * @param [in] c_ident   The candidate ID number
         */
        void ident(std::size_t c_ident);

    private:
        MsecTimeType _pulse_period; // Candidate period (ms)
        SecPerSecType _pulse_pdot;  // Candidate period derivative (sec/sec)
        Dm _dm;                     // Candidate dispersion measure (pc/cm^3)
        std::size_t _harm_num;      // Candidate harmonic number
        MsecTimeType _pulse_width;  // Candidate pulse width (ms)
        T _sigma;                   // Candidate signficance
        std::size_t _ident;         // Candidate ID
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/Candidate.cpp"

#endif // SKA_CHEETAH_DATA_CANDIDATE_H
