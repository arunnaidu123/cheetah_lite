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
#include "cheetah/data/TimeSeries.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename Arch, typename ValueType, typename Alloc>
TimeSeries<Arch, ValueType, Alloc>::TimeSeries(std::size_t size, Allocator const& alloc)
    : BaseT(size, alloc)
    , _sampling_interval(1.0 * second)
{
}

template <typename Arch, typename ValueType, typename Alloc>
TimeSeries<Arch, ValueType, Alloc>::TimeSeries(Allocator const& alloc)
    : TimeSeries(0, alloc)
{
}

template <typename Arch, typename ValueType, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
TimeSeries<Arch, ValueType, Alloc>::TimeSeries(TimeSeries<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator)
    : BaseT(copy, allocator)
    , _sampling_interval(copy.sampling_interval())
{
}

template <typename Arch, typename ValueType, typename Alloc>
TimeSeries<Arch, ValueType, Alloc>::TimeSeries(TimeType const& dt, std::size_t size, Allocator const& alloc)
    : BaseT(size, alloc)
    , _sampling_interval(dt)
{
}

template <typename Arch, typename ValueType, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
TimeSeries<Arch, ValueType, Alloc>& TimeSeries<Arch, ValueType, Alloc>::operator=(TimeSeries<OtherArch, OtherValueT, OtherAlloc> const& copy)
{
    _sampling_interval = copy.sampling_interval();
    BaseT::operator=(copy);
    return *this;
}

template <typename Arch, typename ValueType, typename Alloc>
TimeSeries<Arch, ValueType, Alloc>::~TimeSeries()
{
}

template <typename Arch, typename ValueType, typename Alloc>
TimeType const& TimeSeries<Arch, ValueType, Alloc>::sampling_interval() const
{
    return _sampling_interval;
}

template <typename Arch, typename ValueType, typename Alloc>
void TimeSeries<Arch, ValueType, Alloc>::sampling_interval(TimeType const& dt)
{
    _sampling_interval = dt;
}

// --- specialisations for the data type traits helpers
template<class ArchT, typename ValueT, typename AllocatorT>
struct DataTypeTraits<TimeSeries<ArchT, ValueT, AllocatorT>>
{
    typedef TimeSeries<ArchT, ValueT, AllocatorT> type;
    typedef ArchT Architecture;
    typedef ValueT value_type;
    typedef AllocatorT Allocator;
};

} // namespace data
} // namespace cheetah
} // namespace ska
