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

template<typename Type, typename DerivedType>
DataSequence2DFastIteratorBase<Type, DerivedType>::~DataSequence2DFastIteratorBase()
{
}

template<typename Type, typename DerivedType>
DataSequence2DFastIteratorBase<Type, DerivedType>::DataSequence2DFastIteratorBase()
    :_data(nullptr)
{
}

template<typename Type, typename DerivedType>
DataSequence2DFastIteratorBase<Type, DerivedType>::DataSequence2DFastIteratorBase(const DataSequence2DFastIteratorBase &copy) 
    :_data(copy._data)
    ,_offset(copy._offset)
    ,_raw(copy._raw)
{
}

template<typename Type, typename DerivedType>
DataSequence2DFastIteratorBase<Type, DerivedType>::DataSequence2DFastIteratorBase(
        DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data_ptr,
        std::size_t slow_offset
        )
{
    this->_data = &data_ptr;
    this->_raw = data_ptr.data();
    this->_offset = slow_offset*data_ptr.fast_axis_length();
}

template<typename Type, typename DerivedType>
DataSequence2DFastIteratorBase<Type, DerivedType>::DataSequence2DFastIteratorBase(
        const DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data_ptr,
        std::size_t slow_offset
        )
{
    this->_data = &data_ptr;
    this->_raw = data_ptr.data();
    this->_offset = slow_offset*data_ptr.fast_axis_length();
}



template<typename Type, typename DerivedType>
typename DataSequence2DFastIteratorBase<Type, DerivedType>::PtrType  DataSequence2DFastIteratorBase<Type, DerivedType>::operator->() const{
    BOOST_ASSERT_MSG(this->_raw != nullptr,"Cannot dereference iterator over null DataSequence2D");
    BOOST_ASSERT_MSG(this->_offset < this->_data->fast_axis_length()*this->_data->slow_axis_length(),"Cannot dereference post-end iterator");
    return this->_raw+this->_offset;
}

template<typename Type, typename DerivedType>
typename DataSequence2DFastIteratorBase<Type, DerivedType>::RefType  DataSequence2DFastIteratorBase<Type, DerivedType>::operator*() const{
    BOOST_ASSERT_MSG(this->_raw != nullptr,"Cannot dereference iterator over null DataSequence2D");
    BOOST_ASSERT_MSG(this->_offset < this->_data->fast_axis_length()*this->_data->slow_axis_length(),"Cannot dereference post-end iterator");
    return *(this->_raw+this->_offset);
}

template<typename Type, typename DerivedType>
DerivedType&  DataSequence2DFastIteratorBase<Type, DerivedType>::operator++(){
    ++(this->_offset);
    return static_cast<DerivedType&>(*this);
}


template<typename Type, typename DerivedType>
DerivedType DataSequence2DFastIteratorBase<Type, DerivedType>::operator++(int){
    DerivedType r(static_cast<DerivedType const&>(*this));
    ++(this->_offset);
    return r;
}

template<typename Type, typename DerivedType>
DerivedType&  DataSequence2DFastIteratorBase<Type, DerivedType>::operator--(){
    --(this->_offset);
    return *this;
}

template<typename Type, typename DerivedType>
DerivedType DataSequence2DFastIteratorBase<Type, DerivedType>::operator--(int){
    DerivedType r(static_cast<DerivedType const&>(*this));
    --(this->_offset);
    return r;
}

template<typename Type, typename DerivedType>
DerivedType&  DataSequence2DFastIteratorBase<Type, DerivedType>::operator+=(typename DataSequence2DFastIteratorBase<Type, DerivedType>::DiffType off){
    this->_offset += off;
    return static_cast<DerivedType&>(*this);
}

template<typename Type, typename DerivedType>
DerivedType& DataSequence2DFastIteratorBase<Type, DerivedType>::operator-=(typename DataSequence2DFastIteratorBase<Type, DerivedType>::DiffType off){
    this->_offset -= off;
    return static_cast<DerivedType&>(*this);
}

template<typename Type, typename DerivedType>
DerivedType DataSequence2DFastIteratorBase<Type, DerivedType>::operator+(typename DataSequence2DFastIteratorBase<Type, DerivedType>::DiffType off) const
{
    DerivedType r(static_cast<DerivedType const&>(*this));
    r._offset += off;
    return r;
}

template<typename Type, typename DerivedType>
DerivedType DataSequence2DFastIteratorBase<Type, DerivedType>::operator-(typename DataSequence2DFastIteratorBase<Type, DerivedType>::DiffType off) const
{
    DerivedType r(static_cast<DerivedType const&>(*this));
    r._offset -= off;
    return r;
}

template<typename Type, typename DerivedType>
typename DataSequence2DFastIteratorBase<Type, DerivedType>::DiffType DataSequence2DFastIteratorBase<Type, DerivedType>::operator-(const DataSequence2DFastIteratorBase<Type, DerivedType> &right) const
{
    return this->_offset - right._offset;
}

template<typename Type, typename DerivedType>
bool DataSequence2DFastIteratorBase<Type, DerivedType>::operator<(const SelfType& right) const
{
    return this->_offset < right._offset;
}

template<typename Type, typename DerivedType>
bool DataSequence2DFastIteratorBase<Type, DerivedType>::operator>(const SelfType& right) const
{
    return this->_offset > right._offset;
}

template<typename Type, typename DerivedType>
bool DataSequence2DFastIteratorBase<Type, DerivedType>::operator<=(const SelfType& right) const
{
    return this->_offset <= right._offset;
}

template<typename Type, typename DerivedType>
bool DataSequence2DFastIteratorBase<Type, DerivedType>::operator>=(const SelfType& right) const {
    return (this->_data==right._data) && (this->_offset >= right._offset);
}

template<typename Type, typename DerivedType>
typename DataSequence2DFastIteratorBase<Type, DerivedType>::RefType DataSequence2DFastIteratorBase<Type, DerivedType>::operator[](typename DataSequence2DFastIteratorBase<Type, DerivedType>::DiffType off) const {
    return *((*this)+off);
}

template<typename Type, typename DerivedType>
bool DataSequence2DFastIteratorBase<Type, DerivedType>::operator==(DataSequence2DFastIteratorBase<Type, DerivedType> const& it) const
{
    return (it._data == _data) && (it._offset == _offset);
}

template<typename Type, typename DerivedType>
bool DataSequence2DFastIteratorBase<Type, DerivedType>::operator!=(DataSequence2DFastIteratorBase<Type, DerivedType> const& it) const
{
    return (it._data == _data) && (it._offset != _offset);
}

template<typename Type, typename DerivedType>
bool operator==(DataSequence2DFastIteratorBase<Type, DerivedType> const& it1, DataSequence2DFastIteratorBase<Type, DerivedType> const& it2){
    return it1.operator==(it2);
}

template<typename Type, typename DerivedType>
bool operator!=(DataSequence2DFastIteratorBase<Type, DerivedType> const& it1, DataSequence2DFastIteratorBase<Type, DerivedType> const& it2){
    return it1.operator!=(it2);
}


} // namespace data
} // namespace cheetah
} // namespace ska

