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
#include "cheetah/data/VectorLike.h"
#include <utility>

namespace ska {
namespace cheetah {
namespace data {

template <typename VectorType>
VectorLike<VectorType>::VectorLike()
{
}

template <typename VectorType>
VectorLike<VectorType>::VectorLike(Allocator const& allocator)
    : _data(allocator)
{
}

template <typename VectorType>
VectorLike<VectorType>::~VectorLike()
{
}

template <typename VectorType>
template<typename... Args>
VectorLike<VectorType>::VectorLike(std::size_t n, Args&&... args)
    : _data(n, std::forward<Args>(args)...)
{
}

template <typename VectorType>
VectorLike<VectorType>::VectorLike(std::size_t n, const ValueType& value, Allocator const& allocator)
    : _data(n,value, allocator)
{
}

template <typename VectorType>
std::size_t VectorLike<VectorType>::size(void) const
{
    return _data.size();
}

template <typename VectorType>
void VectorLike<VectorType>::resize(std::size_t n, const ValueType &x)
{
    _data.resize(n,x);
}

template <typename VectorType>
typename VectorLike<VectorType>::Reference VectorLike<VectorType>::operator[](std::size_t n)
{
    return _data[n];
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstReference VectorLike<VectorType>::operator[](std::size_t n) const
{
    return _data[n];
}

template <typename VectorType>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::begin()
{
    return _data.begin();
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::begin() const
{
    return _data.cbegin();
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::cbegin() const
{
    return _data.cbegin();
}

template <typename VectorType>
typename VectorLike<VectorType>::ReverseIterator VectorLike<VectorType>::rbegin()
{
    return _data.rbegin();
}

template <typename VectorType>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::end()
{
    return _data.end();
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::end() const
{
    return _data.cend();
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::cend() const
{
    return _data.cend();
}

template <typename VectorType>
typename VectorLike<VectorType>::ReverseIterator VectorLike<VectorType>::rend()
{
    return _data.rend();
}

template <typename VectorType>
typename VectorLike<VectorType>::Pointer VectorLike<VectorType>::data()
{
    return _data.data();
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstPointer VectorLike<VectorType>::data() const
{
    return _data.data();
}

template <typename VectorType>
void VectorLike<VectorType>::push_back(ValueType const& value)
{
    return _data.push_back(value);
}

template <typename VectorType>
template<typename ...Args>
void VectorLike<VectorType>::emplace_back(Args&&... values)
{
    _data.emplace_back(std::forward<Args>(values)...);
}

template <typename VectorType>
void VectorLike<VectorType>::reserve(std::size_t size)
{
    return _data.reserve(size);
}

template <typename VectorType>
std::size_t VectorLike<VectorType>::capacity() const
{
    return _data.capacity();
}

template <typename VectorType>
void VectorLike<VectorType>::swap(VectorLike& v)
{
    return _data.swap(v._data);
}

template <typename VectorType>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::erase(Iterator pos)
{
    return _data.erase(pos);
}

template <typename VectorType>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::erase(Iterator first, Iterator last)
{
    return _data.erase(first,last);
}

template <typename VectorType>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::insert(Iterator pos, const ValueType& value)
{
    return _data.insert(pos, value);
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::insert(ConstIterator pos, ValueType&& value)
{
    return _data.insert(pos, std::move(value));
}

template <typename VectorType>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::insert(ConstIterator pos, const ValueType& value)
{
    return _data.insert(pos, value);
}

template <typename VectorType>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::insert(Iterator pos, ValueType&& value)
{
    return _data.insert(pos, std::move(value));
}

template <typename VectorType>
template<typename...Args>
typename VectorLike<VectorType>::Iterator VectorLike<VectorType>::emplace(Iterator pos, Args&&... values)
{
    return _data.emplace(pos, std::forward<Args>(values)...);
}

template <typename VectorType>
template<typename...Args>
typename VectorLike<VectorType>::ConstIterator VectorLike<VectorType>::emplace(ConstIterator pos, Args&&... values)
{
    return _data.emplace(pos, std::forward<Args>(values)...);
}

template <typename VectorType>
inline typename VectorLike<VectorType>::ValueType& VectorLike<VectorType>::front()
{
    return _data.front();
}

template <typename VectorType>
inline typename VectorLike<VectorType>::ValueType const& VectorLike<VectorType>::front() const
{
    return _data.front();
}

template <typename VectorType>
inline typename VectorLike<VectorType>::ValueType& VectorLike<VectorType>::back()
{
    return _data.back();
}

template <typename VectorType>
inline typename VectorLike<VectorType>::ValueType const& VectorLike<VectorType>::back() const
{
    return _data.back();
}

template <typename VectorType>
inline bool VectorLike<VectorType>::empty() const
{
    return _data.empty();
}

template <typename VectorType>
void VectorLike<VectorType>::clear()
{
    _data.clear();
}

template <typename VectorType>
VectorLike<VectorType>& VectorLike<VectorType>::operator+=(SelfType const& v)
{
    for(auto it=v.begin(); it != v.end(); ++it) {
        _data.push_back(*it);
    }
    return *this;
}

template <typename VectorType>
typename VectorLike<VectorType>::Allocator VectorLike<VectorType>::allocator() const
{
    return _data.get_allocator();
}


} // namespace data
} // namespace cheetah
} // namespace ska
