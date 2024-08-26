/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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
#ifndef SKA_CHEETAH_DATA_NUDOT_H
#define SKA_CHEETAH_DATA_NUDOT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpragmas"
#include <boost/units/quantity.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/io.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/physical_dimensions/length.hpp>
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace data {

typedef boost::units::derived_dimension<boost::units::time_base_dimension, -2>::type NuDotDimensionType;
typedef boost::units::unit<NuDotDimensionType, boost::units::si::system> NuDotUnit;
BOOST_UNITS_STATIC_CONSTANT(per_square_hertz, NuDotUnit);

template<typename T> using NuDotType = boost::units::quantity<NuDotUnit, T>;

} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_NUDOT_H
