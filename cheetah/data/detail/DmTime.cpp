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
#include "cheetah/data/DmTime.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

/**
 * @brief      Functor to support comparison of start times in DmTime objects
 */
template <typename DmTrialsType>
struct DmTimeStartTimeComparitor
{
    typedef std::shared_ptr<DmTrialsType> ElementType;
    bool operator()(const ElementType &lhs, const ElementType &rhs) const;
};

template <typename DmTrialsType>
bool DmTimeStartTimeComparitor<DmTrialsType>::operator()(const ElementType &lhs, const ElementType &rhs) const
{
    return lhs->start_time() < rhs->start_time();
}

} // namespace detail

template <typename DmTrialsType>
DmTime<DmTrialsType>::DmTime()
: _ready(false)
, _dump_time(540.0*data::seconds)
{
}

template <typename DmTrialsType>
DmTime<DmTrialsType>::~DmTime()
{
}

template <typename DmTrialsType>
void DmTime<DmTrialsType>::add(ValueType data)
{
    //First determine where in the list this block should be
    auto it = std::upper_bound(_data.begin(),_data.end(),data,detail::DmTimeStartTimeComparitor<DmTrialsType>());

    //Insert it in an ordered manner
    _data.insert(it,data);
}

template <typename DmTrialsType>
void DmTime<DmTrialsType>::clear()
{
    _data.clear();
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::Iterator DmTime<DmTrialsType>::begin(std::size_t number_dms_per_slice)
{
    return Iterator(0, number_dms_per_slice, this->shared_from_this());
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::ConstIterator DmTime<DmTrialsType>::cbegin(std::size_t number_dms_per_slice) const
{
    return ConstIterator(0, number_dms_per_slice, this->shared_from_this());
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::Iterator DmTime<DmTrialsType>::end()
{
    if (_data.empty())
        return Iterator(0,0,this->shared_from_this());
    return Iterator(_data.front()->size(),0,this->shared_from_this());
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::ConstIterator DmTime<DmTrialsType>::cend() const
{
    if (_data.empty())
        return ConstIterator(0,0,this->shared_from_this());
    return ConstIterator(_data.front()->size(),0,this->shared_from_this());
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::ContainerType const& DmTime<DmTrialsType>::blocks() const
{
    return _data;
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::ContainerType& DmTime<DmTrialsType>::blocks()
{
    return _data;
}

template <typename DmTrialsType>
void DmTime<DmTrialsType>::ready()
{
    std::lock_guard lk(_m);
    _ready = true;
    _cv.notify_all();
}

template <typename DmTrialsType>
void DmTime<DmTrialsType>::wait()
{
    std::unique_lock lk(_m);
    _cv.wait(lk, [this]{ return _ready; });
}

template <typename DmTrialsType>
typename DmTime<DmTrialsType>::Seconds DmTime<DmTrialsType>::dump_time() const
{
    return _dump_time;
}

template <typename DmTrialsType>
void DmTime<DmTrialsType>::dump_time(typename DmTime<DmTrialsType>::Seconds dump_time)
{
    _dump_time = dump_time;
}

} // namespace data
} // namespace cheetah
} // namespace ska
