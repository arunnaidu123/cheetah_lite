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
#include <boost/assert.hpp>

namespace ska {
namespace cheetah {
namespace data {

template<typename Type>
DataSequence2DPartialIterator<Cpu,Type>::DataSequence2DPartialIterator()
    : BaseT()
    , _fbegin(0)
    , _fend(0)
    , _sbegin(0)
    , _send(0)
    , _flength(0)
{
}

template<typename Type>
DataSequence2DPartialIterator<Cpu,Type>::~DataSequence2DPartialIterator() {
}


template<typename Type>
DataSequence2DPartialIterator<Cpu,Type>::DataSequence2DPartialIterator(const DataSequence2DPartialIterator<Cpu,Type> &copy) 
    : BaseT(copy) 
    , _fbegin(copy._fbegin)
    , _fend(copy._fend)
    , _sbegin(copy._sbegin)
    , _send(copy._send)
{
    this->_flength = this->_fend - this->_fbegin;
}

template<typename Type>
DataSequence2DPartialIterator<Cpu,Type>::DataSequence2DPartialIterator(
        const DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data_ptr,
        std::size_t fbegin, std::size_t fend, std::size_t sbegin, std::size_t send)
    : BaseT(data_ptr)
    , _fbegin(fbegin)
    , _fend(fend)
    , _sbegin(sbegin)
    , _send(send)
{
    this->_flength = this->_fend - this->_fbegin;
}


template<typename Type>
DataSequence2DPartialIterator<Cpu,Type>::DataSequence2DPartialIterator(
        DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data_ptr,
        std::size_t fbegin, std::size_t fend, std::size_t sbegin, std::size_t send)
    : BaseT(data_ptr)
    , _fbegin(fbegin)
    , _fend(fend)
    , _sbegin(sbegin)
    , _send(send)
{
    this->_flength = this->_fend - this->_fbegin;
}


template<typename Type>
typename DataSequence2DPartialIterator<Cpu,Type>::PtrType  DataSequence2DPartialIterator<Cpu,Type>::operator->() const{
    BOOST_ASSERT_MSG(this->_raw != nullptr,"Cannot dereference iterator over null DataSequence2D");
    BOOST_ASSERT_MSG(this->mangle_offset(this->_offset) < this->_data->fast_axis_length()*this->_data->slow_axis_length(),"Cannot dereference post-end iterator");
    return this->_raw+this->mangle_offset(this->_offset);
}

template<typename Type>
typename DataSequence2DPartialIterator<Cpu,Type>::RefType  DataSequence2DPartialIterator<Cpu,Type>::operator*() const{
    BOOST_ASSERT_MSG(this->_raw != nullptr,"Cannot dereference iterator over null DataSequence2D");
    BOOST_ASSERT_MSG(this->mangle_offset(this->_offset) < this->_data->fast_axis_length()*this->_data->slow_axis_length(),"Cannot dereference post-end iterator");
    return *(this->_raw+this->mangle_offset(this->_offset));
}

template<typename Type>
std::size_t DataSequence2DPartialIterator<Cpu,Type>::mangle_offset(std::size_t off) const {
    std::size_t s_idx = off / this->_flength;
    std::size_t f_idx = off - s_idx * this->_flength;
    return (this->_sbegin+s_idx) * this->_data->fast_axis_length() + this->_fbegin + f_idx;
}

template<typename Type>
DataSequence2DPartialIterator<Cpu,Type> DataSequence2DPartialIterator<Cpu,Type>::begin() const {
    DataSequence2DPartialIterator<Cpu,Type> r(*this);
    r._offset=0;
    return r;
}

template<typename Type>
DataSequence2DPartialIterator<Cpu,Type> DataSequence2DPartialIterator<Cpu,Type>::end() const {
    DataSequence2DPartialIterator<Cpu,Type> r(*this);
    r._offset=this->_flength*(this->_send - this->_sbegin);
    return r;
}

} // namespace data
} // namespace cheetah
} // namespace ska

