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
#include "cheetah/data/FrequencySeries.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename Arch, typename ValueType, typename Alloc>
FrequencySeries<Arch, ValueType, Alloc>::FrequencySeries(Allocator const& alloc)
    : BaseT(0, alloc)
    , _frequency_step(1.0 * hz)
{
}

template <typename Arch, typename ValueType, typename Alloc>
FrequencySeries<Arch, ValueType, Alloc>::FrequencySeries(std::size_t size, Allocator const& alloc)
    : BaseT(size, alloc)
    , _frequency_step(1.0 * hz)
{
}

template <typename Arch, typename ValueType, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
FrequencySeries<Arch, ValueType, Alloc>::FrequencySeries(FrequencySeries<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator)
    : BaseT(copy, allocator)
    , _frequency_step(copy.frequency_step())
{
}

template <typename Arch, typename ValueType, typename Alloc>
FrequencySeries<Arch, ValueType, Alloc>::FrequencySeries(FourierFrequencyType const& df, std::size_t size, Allocator const& allocator)
    : BaseT(size, allocator)
    , _frequency_step(df)
{
}

template <typename Arch, typename ValueType, typename Alloc>
FrequencySeries<Arch, ValueType, Alloc>::FrequencySeries(FourierFrequencyType const& df, Allocator const& allocator)
    : FrequencySeries(df, 0, allocator)
{
}

template <typename Arch, typename ValueType, typename Alloc>
FrequencySeries<Arch, ValueType, Alloc>::~FrequencySeries()
{
}

template <typename Arch, typename ValueType, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
FrequencySeries<Arch, ValueType, Alloc>& FrequencySeries<Arch, ValueType, Alloc>::operator=(FrequencySeries<OtherArch, OtherValueT, OtherAlloc> const& copy)
{
    _frequency_step = copy.frequency_step();
    BaseT::operator=(copy);
    return *this;
}

template <typename Arch, typename ValueType, typename Alloc>
FourierFrequencyType const& FrequencySeries<Arch, ValueType, Alloc>::frequency_step() const
{
    return _frequency_step;
}

template <typename Arch, typename ValueType, typename Alloc>
void FrequencySeries<Arch, ValueType, Alloc>::frequency_step(FourierFrequencyType const& df)
{
    _frequency_step = df;
}

template <typename Arch, typename ValueType, typename Alloc>
data::FourierFrequencyType const FrequencySeries<Arch, ValueType, Alloc>::bin_to_frequency(std::size_t idx) const
{
    return data::FourierFrequencyType(((idx * _frequency_step.value()) + (_frequency_step.value()/2.0f)) * data::hz);
}

template <typename Arch, typename ValueType, typename Alloc>
data::TimeType const FrequencySeries<Arch, ValueType, Alloc>::bin_to_period(std::size_t idx) const
{
    return data::TimeType((1.0f / bin_to_frequency(idx).value()) * data::seconds);
}

template <typename Arch, typename ValueType, typename Alloc>
std::size_t FrequencySeries<Arch, ValueType, Alloc>::frequency_to_bin(data::FourierFrequencyType const& frequency) const
{
    return (std::size_t)(frequency.value()/_frequency_step.value() + 0.5f);
}

// --- specialisations for the data type traits helpers
template<class ArchT, typename ValueT, typename AllocatorT>
struct DataTypeTraits<FrequencySeries<ArchT, ValueT, AllocatorT>>
{
    typedef FrequencySeries<ArchT, ValueT, AllocatorT> type;
    typedef ArchT Architecture;
    typedef ValueT value_type;
    typedef AllocatorT Allocator;
};

} // namespace data
} // namespace cheetah
} // namespace ska
