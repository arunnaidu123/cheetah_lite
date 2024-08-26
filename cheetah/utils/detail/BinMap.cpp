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
#include "cheetah/utils/BinMap.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/units/quantity.hpp>
#pragma GCC diagnostic pop
#include <functional>

namespace ska {
namespace cheetah {
namespace utils {

namespace detail {
template<typename DataType>
struct DataTypeOperationsHelper {
    template<typename T>
    inline
    DataType operator()(DataType const& data, T const& t) const {
        return data * t;
    }
    template<typename T>
    inline
    DataType divide(DataType const& data, T const& t) const {
        return data/DataType(t);
    }
};

template<typename Unit, typename DataType>
struct DataTypeOperationsHelper<boost::units::quantity<Unit, DataType>> {
    template<typename T>
    inline
    boost::units::quantity<Unit,DataType> operator()(boost::units::quantity<Unit,DataType> const& data, T const& t) const {
        return data * static_cast<DataType>(t);
    }
    template<typename T>
    inline
    boost::units::quantity<Unit,DataType> divide(boost::units::quantity<Unit,DataType> const& data, T const& t) const {
        return data/static_cast<DataType>(t);
    }
};

} // namespace detail

template<typename DataType>
BinMap<DataType>::BinMap()
    : _n_bins(0)
    , _lower(0)
    , _width(0)
{
}

template<typename DataType>
BinMap<DataType>::~BinMap()
{
}

template<typename DataType>
BinMap<DataType>::BinMap(unsigned int number_of_bins)
    : _n_bins(number_of_bins)
    , _lower(0)
    , _width(0)
{
}

template<typename DataType>
BinMap<DataType>::BinMap(unsigned int number_of_bins, DataType start, DataType const& end)
    : _n_bins(number_of_bins)
    , _lower(std::move(start))
{
    set_upper_bound(end);
}

template<typename DataType>
void BinMap<DataType>::reset(unsigned int number_of_bins)
{
     _n_bins = number_of_bins;
}

template<typename DataType>
void BinMap<DataType>::set_bin_width(DataType width)
{
    _width = width;
    _halfwidth = detail::DataTypeOperationsHelper<DataType>().divide(width, 2);
}

template<typename DataType>
void BinMap<DataType>::set_bounds(DataType lower, DataType upper)
{
    _lower = lower;
    set_upper_bound(upper);
}

template<typename DataType>
void BinMap<DataType>::set_lower_bound(DataType const start)
{
    _lower = start;
}

template<typename DataType>
void BinMap<DataType>::set_upper_bound(DataType const& end)
{
    set_bin_width( detail::DataTypeOperationsHelper<DataType>().divide((end - _lower),_n_bins));
}

template<typename DataType>
inline DataType BinMap<DataType>::lower_bound() const
{
    return _lower;
}

template<typename DataType>
inline DataType BinMap<DataType>::upper_bound() const
{
    return last_bin_assignment_value() + _halfwidth;
}

template<typename DataType>
inline DataType BinMap<DataType>::bin_assignment_number(int index) const
{
    return _lower + _halfwidth + detail::DataTypeOperationsHelper<DataType>()(_width, index);
}

template<typename DataType>
DataType BinMap<DataType>::bin_start(unsigned int index) const
{
   return bin_assignment_number(index) - _halfwidth;
}

template<typename DataType>
DataType BinMap<DataType>::bin_end(unsigned int index) const
{
   return bin_assignment_number(index) + _halfwidth;
}

template<typename DataType>
unsigned int BinMap<DataType>::bin_index(DataType const& value) const
{
    return (int)(0.5 + ((value - _lower)/ _width ));
}

template<typename DataType>
inline unsigned BinMap<DataType>::last_bin_index() const
{
    return _n_bins-1;
}

template<typename DataType>
DataType BinMap<DataType>::last_bin_assignment_value() const 
{ 
    return _lower + detail::DataTypeOperationsHelper<DataType>()(_width, last_bin_index()) + _halfwidth;
}

template<typename DataType>
DataType BinMap<DataType>::first_bin_assignment_value() const 
{ 
    return _lower + _halfwidth;
}

template<typename DataType>
DataType BinMap<DataType>::bin_width() const 
{
    return _width;
}

template<typename DataType>
unsigned int BinMap<DataType>::number_of_bins() const
{
    return _n_bins;
}

template<typename DataType>
bool BinMap<DataType>::equal(const BinMap<DataType>& map) const
{
     return map._lower == _lower && map._n_bins == _n_bins && map._width == _width;
}

template<typename DataType>
bool operator==(const BinMap<DataType>& map_1, const BinMap<DataType>& map_2)
{
     return map_1.equal(map_2);
}

} // namespace utils
} // namespace cheetah
} // namespace ska

// hashing function to allow storage of type in STL containers
namespace std {
    template<typename DataType>
    struct hash<ska::cheetah::utils::BinMap<DataType>> {
        std::size_t operator()(const ska::cheetah::utils::BinMap<DataType>& map) const {
            return std::hash<unsigned>()(map.number_of_bins()) ^ std::hash<DataType>()(map.lower_bound()) ^ (std::hash<DataType>()(map.bin_width()) << 4);
        }
    };
} // namespace std
