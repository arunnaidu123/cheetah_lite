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
#include "cheetah/data/DataSequence2D.h"

namespace ska {
namespace cheetah {
namespace data {

DataSequence2D<Cpu,bool>::DataSequence2D()
    : _fast_axis_length(0)
    , _slow_axis_length(0)
{}

DataSequence2D<Cpu,bool>::DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length)
    : _fast_axis_length(fast_axis_length)
    , _slow_axis_length(slow_axis_length)
    , _data()
{
    this->_data.resize(fast_axis_length*slow_axis_length);
}

DataSequence2D<Cpu,bool>::DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length, const bool &fill)
    : _fast_axis_length(fast_axis_length)
    , _slow_axis_length(slow_axis_length)
    , _data(fast_axis_length*slow_axis_length,fill)
{
}

DataSequence2D<Cpu,bool>::~DataSequence2D(){}

void DataSequence2D<Cpu,bool>::resize(std::size_t fast_axis_length, std::size_t slow_axis_length, const bool &fill)
{
    this->_fast_axis_length = fast_axis_length;
    this->_slow_axis_length = slow_axis_length;
    this->_data.resize(this->_fast_axis_length*this->_slow_axis_length);
    std::fill(this->_data.begin(),this->_data.end(), fill);
}

void DataSequence2D<Cpu,bool>::resize(std::size_t fast_axis_length, std::size_t slow_axis_length)
{
    this->_fast_axis_length = fast_axis_length;
    this->_slow_axis_length = slow_axis_length;
    this->_data.resize(this->_fast_axis_length*this->_slow_axis_length);
}

typename DataSequence2D<Cpu,bool>::Iterator DataSequence2D<Cpu,bool>::begin() {
    return _data.begin();
}

typename DataSequence2D<Cpu,bool>::ConstIterator DataSequence2D<Cpu,bool>::begin() const {
    return _data.begin();
}

typename DataSequence2D<Cpu,bool>::ConstIterator DataSequence2D<Cpu,bool>::cbegin() const {
    return _data.cbegin();
}


typename DataSequence2D<Cpu,bool>::Iterator DataSequence2D<Cpu,bool>::end() {
    return _data.end();
}

typename DataSequence2D<Cpu,bool>::ConstIterator DataSequence2D<Cpu,bool>::end() const {
    return _data.end();
}

typename DataSequence2D<Cpu,bool>::ConstIterator DataSequence2D<Cpu,bool>::cend() const {
    return _data.cend();
}

typename DataSequence2D<Cpu,bool>::PartialRange DataSequence2D<Cpu,bool>::slice(
        std::size_t fbegin,
        std::size_t fend,
        std::size_t sbegin,
        std::size_t send)
{
    BOOST_ASSERT(fend <= this->fast_axis_length());
    BOOST_ASSERT(send <= this->slow_axis_length());

    return DataSequence2DPartialRange<Cpu,bool>(*this, fbegin, fend, sbegin, send, _fast_axis_length);
}

typename DataSequence2D<Cpu,bool>::ConstPartialRange DataSequence2D<Cpu,bool>::slice(
        std::size_t fbegin,
        std::size_t fend,
        std::size_t sbegin,
        std::size_t send) const
{
    BOOST_ASSERT(fend <= this->fast_axis_length());
    BOOST_ASSERT(send <= this->slow_axis_length());

    return DataSequence2D<Cpu,bool>::ConstPartialRange(*this, fbegin, fend, sbegin, send, _fast_axis_length);
}

} // namespace data
} // namespace cheetah
} // namespace ska
