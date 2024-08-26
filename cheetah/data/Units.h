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
#ifndef SKA_CHEETAH_DATA_UNITS_H
#define SKA_CHEETAH_DATA_UNITS_H

#include <pss/astrotypes/units/Units.h>
#include <pss/astrotypes/multiarray/DimensionSpan.h>
#include <pss/astrotypes/multiarray/DimensionSize.h>
#include <pss/astrotypes/multiarray/DimensionIndex.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/systems/si/frequency.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/acceleration.hpp>
#include <boost/units/make_scaled_unit.hpp>
#include <boost/units/io.hpp>
#include <boost/units/systems/si/codata_constants.hpp>
#include <boost/chrono/chrono.hpp>
#pragma GCC diagnostic pop


namespace ska {
namespace cheetah {
namespace data {

    using pss::astrotypes::units::Frequency;
    using pss::astrotypes::units::Time;
    template<typename T> using DimensionSize = pss::astrotypes::DimensionSize<T>;
    template<typename T> using DimensionIndex = pss::astrotypes::DimensionIndex<T>;
    template<typename T> using DimensionSpan = pss::astrotypes::DimensionSpan<T>;

    // units to be used in boost::quantity<unit, Rep> types
    typedef typename pss::astrotypes::units::Hertz Hertz;
    typedef typename pss::astrotypes::units::KiloHertz KiloHertz;
    typedef typename pss::astrotypes::units::MegaHertz MegaHertz;
    typedef typename pss::astrotypes::units::Seconds Seconds;
    typedef typename pss::astrotypes::units::MilliSeconds MilliSeconds;
    typedef typename pss::astrotypes::units::MicroSeconds MicroSeconds;
    typedef typename pss::astrotypes::units::NanoSeconds NanoSeconds;

    static boost::units::si::frequency const hz = boost::units::si::hertz;
    static boost::units::si::frequency const hertz = boost::units::si::hertz;
    static const pss::astrotypes::units::MegaHertz megahertz = pss::astrotypes::units::megahertz;

    static boost::units::si::time const second = boost::units::si::second;
    static boost::units::si::time const seconds = boost::units::si::second;
    static const MilliSeconds millisecond;
    static const MilliSeconds milliseconds;
    static const MicroSeconds microsecond;
    static const MicroSeconds microseconds;
    static boost::units::si::acceleration const metres_per_second_squared = boost::units::si::metres_per_second_squared;
    static boost::units::si::acceleration const metre_per_second_squared = boost::units::si::metres_per_second_squared;
    static boost::units::si::acceleration const meter_per_second_squared = boost::units::si::metres_per_second_squared;
    static boost::units::si::acceleration const meters_per_second_squared = boost::units::si::metres_per_second_squared;

    // add typedefs for commonly used quantities here
    typedef boost::units::quantity<Hertz, double> FourierFrequencyType; //Hz used for fluctuation frequencies
    typedef boost::units::quantity<boost::units::si::time, double> TimeType; //s
    typedef boost::units::quantity<boost::units::si::acceleration, double> AccelerationType; //m/s/s
    //typedef boost::units::quantity<boost::units::si::dimensionless, float> SigmaType; // sigma of candidate
    typedef boost::units::quantity<MegaHertz, double> FrequencyType; /* used for observing frequencies */

    template <typename T>
    class SecondsType : public boost::units::quantity<boost::units::si::time, T>
    {
            typedef boost::units::quantity<boost::units::si::time, T> BaseT;

        public:
            SecondsType() {}

            template<typename Unit, typename YY>
            SecondsType(boost::units::quantity<Unit, YY> const& arg) : BaseT(arg) {}

            /// conversion from std::chrono::duration types
            template<typename DurationRep, typename Period>
            explicit SecondsType(std::chrono::duration<DurationRep, Period> const& d)
                : BaseT(std::chrono::duration_cast<std::chrono::duration<DurationRep, std::ratio<1,1>>>(d).count() * seconds)
            {
            }
    };

} // namespace data
} // namespace cheetah
} // namespace ska

namespace boost {
namespace units {
    template<typename Unit, typename X, typename Y>
    inline typename multiply_typeof_helper< X, quantity< Unit, X > >::type
    operator*(const Y & lhs, const quantity< Unit, X > & rhs)
    {
        return static_cast<X>(lhs) * rhs;
    }

    template<typename Unit, typename X, typename Y>
    inline typename divide_typeof_helper< X, quantity< Unit, X > >::type
    operator/(const Y & lhs, const quantity< Unit, X > & rhs)
    {
        return static_cast<X>(lhs) / rhs;
    }
} // namespace units
} // namespace boost

#endif // SKA_CHEETAH_DATA_UNITS_H
