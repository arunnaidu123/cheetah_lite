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
#ifndef SKA_CHEETAH_DATA_DMCONSTANT_H
#define SKA_CHEETAH_DATA_DMCONSTANT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpragmas"
#include <boost/units/quantity.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/io.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/systems/cgs/length.hpp>
#include <boost/units/systems/cgs/time.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/cgs/frequency.hpp>
#include <boost/units/physical_dimensions/length.hpp>
#include <boost/units/physical_dimensions/frequency.hpp>
#include <boost/units/physical_dimensions/time.hpp>
#include <boost/units/base_units/astronomical/parsec.hpp>
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace data {
namespace dm_constant {

/**
 * @brief
 *    Representation of the dispersion measure constant in terms of dimensions and units
 *
 * @details
 *    Implemented using the boost units library. The boost::units::quantity<dm_constant::Unit, double> will be a boost::units::quantity
 *    with the base unit MHz^2parsec^-1cm^3s.
 *    Note you must specify the units when initialising the variable.
 *
 * @example
 * @code
 * #include "cheetah/data/DmConstant.h"
 *
 * boost::units::quantity<data::dm_constant::s_mhz::Unit, double><double> dm_constant = 4.1493775933609e3 * data::dm_constant::s_mhz_squared_cm_cubed_per_pc)
 * boost::units::quantity<data::dm_constant::ms_mhz::Unit, double><double> dm_constant = 4.1493775933609e6 * data::dm_constant::ms_mhz_squared_cm_cubed_per_pc)
 *
 * @endcode
 *
 */


typedef boost::units::scaled_base_unit<boost::units::si::second_base_unit, boost::units::scale<10, boost::units::static_rational<6> > > megaseconds_base_unit;

namespace s_mhz {

typedef boost::units::derived_dimension<
        megaseconds_base_unit,-2,
        boost::units::astronomical::parsec_base_unit, -1,
        boost::units::length_base_dimension, 3,
        boost::units::time_base_dimension,1
        >::type DmConstantDimensionType1;
typedef boost::units::unit<DmConstantDimensionType1, boost::units::cgs::system> Unit;


} // namespace s_mhz

namespace ms_mhz {

typedef boost::units::scaled_base_unit<boost::units::si::second_base_unit, boost::units::scale<10, boost::units::static_rational<-3> > > ms_base_unit;

typedef boost::units::derived_dimension<
        megaseconds_base_unit,-2,
        boost::units::astronomical::parsec_base_unit, -1,
        boost::units::length_base_dimension, 3,
        ms_base_unit,1
        >::type DmConstantDimensionType2;
typedef boost::units::unit<DmConstantDimensionType2, boost::units::cgs::system> Unit;


} // namespace ms_mhz

BOOST_UNITS_STATIC_CONSTANT(s_mhz_squared_cm_cubed_per_pc, s_mhz::Unit);
BOOST_UNITS_STATIC_CONSTANT(ms_mhz_squared_cm_cubed_per_pc, ms_mhz::Unit);

namespace s_mhz {
    static const boost::units::quantity<Unit, double> dm_constant = 4.1493775933609e3 * s_mhz_squared_cm_cubed_per_pc;
}

namespace ms_mhz {
    static const boost::units::quantity<Unit, double> dm_constant = 4.1493775933609e6 * ms_mhz_squared_cm_cubed_per_pc;
}

} // namespace dm_constant
} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_DMCONSTANT_H
