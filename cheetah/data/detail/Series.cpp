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
#include "cheetah/data/DataTypeTraits.h"
#include "panda/Copy.h"


namespace ska {
namespace cheetah {
namespace data {


template <typename Arch, typename ValueT, typename Alloc>
Series<Arch, ValueT, Alloc>::Series(Allocator const& allocator)
    : Series(0, allocator)
{
}

template <typename Arch, typename ValueT, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<Arch, ValueT, Alloc>::Series( Series<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator)
    : _data(copy.size(), allocator)
{
    panda::copy(copy.begin(), copy.end(), _data.begin());
}

template <typename Arch, typename ValueT, typename Alloc>
Series<Arch, ValueT, Alloc>::Series(Series const& copy)
    : _data(copy._data)
{
}

template <typename Arch, typename ValueT, typename Alloc>
Series<Arch, ValueT, Alloc>::Series(Series&& to_move)
    : _data(std::move(to_move))
{
}

template <typename Arch, typename ValueT, typename Alloc>
Series<Arch, ValueT, Alloc>::Series(std::size_t size, Allocator const& allocator)
    : _data(size, allocator)
{
}

template <typename Arch, typename ValueT, typename Alloc>
Series<Arch, ValueT, Alloc>& Series<Arch, ValueT, Alloc>::operator=( Series const& copy)
{
    this->resize(copy.size());
    panda::copy(copy.begin(), copy.end(), _data.begin());
    return *this;
}

template <typename Arch, typename ValueT, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<Arch, ValueT, Alloc>& Series<Arch, ValueT, Alloc>::operator=( Series<OtherArch, OtherValueT, OtherAlloc> const& copy)
{
    this->resize(copy.size());
    panda::copy(copy.begin(), copy.end(), _data.begin());
    return *this;
}

template <typename Arch, typename ValueT, typename Alloc>
inline
typename Series<Arch, ValueT, Alloc>::ConstIterator Series<Arch, ValueT, Alloc>::begin() const
{
    return _data.begin();
}

template <typename Arch, typename ValueT, typename Alloc>
inline
typename Series<Arch, ValueT, Alloc>::ConstIterator Series<Arch, ValueT, Alloc>::cbegin() const
{
    return _data.begin();
}

template <typename Arch, typename ValueT, typename Alloc>
inline
typename Series<Arch, ValueT, Alloc>::Iterator Series<Arch, ValueT, Alloc>::begin()
{
    return _data.begin();
}

template <typename Arch, typename ValueT, typename Alloc>
inline
typename Series<Arch, ValueT, Alloc>::ConstIterator Series<Arch, ValueT, Alloc>::end() const
{
    return _data.end();
}

template <typename Arch, typename ValueT, typename Alloc>
inline
typename Series<Arch, ValueT, Alloc>::Iterator Series<Arch, ValueT, Alloc>::end()
{
    return _data.end();
}

template <typename Arch, typename ValueT, typename Alloc>
inline
typename Series<Arch, ValueT, Alloc>::ConstIterator Series<Arch, ValueT, Alloc>::cend() const
{
    return _data.cend();
}

template <typename Arch, typename ValueT, typename Alloc>
inline
std::size_t Series<Arch, ValueT, Alloc>::size() const
{
    return _data.size();
}

template <typename Arch, typename ValueT, typename Alloc>
void Series<Arch, ValueT, Alloc>::resize(std::size_t size)
{
    if(_data.size() == size) return;
    _data.reset(size);
}

// ---------- CPU Specialisation -------------------------
template <typename ValueT, typename Alloc>
Series<cheetah::Cpu, ValueT, Alloc>::Series()
    : BaseT()
{
}

template <typename ValueT, typename Alloc>
Series<cheetah::Cpu, ValueT, Alloc>::Series(Allocator const& allocator)
    : BaseT(allocator)
{
}

template <typename ValueT, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<cheetah::Cpu, ValueT, Alloc>::Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy)
    : BaseT(copy.size(), ValueT(), Alloc())
{
    panda::copy(copy.begin(), copy.end(), this->begin());
}

template <typename ValueT, typename Alloc>
Series<cheetah::Cpu, ValueT, Alloc>::Series(std::size_t size, Allocator const& allocator)
    : BaseT(size, ValueT(), allocator)
{
}

template <typename ValueT, typename Alloc>
Series<cheetah::Cpu, ValueT, Alloc>::Series(Series const& copy)
    : BaseT(copy.size(), ValueT(), copy.allocator())
{
    panda::copy(copy.begin(), copy.end(), this->begin());
}

template <typename ValueT, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<cheetah::Cpu, ValueT, Alloc>::Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator)
    : BaseT(copy.size(), ValueT(), allocator)
{
    panda::copy(copy.begin(), copy.end(), this->begin());
}

template <typename ValueT, typename Alloc>
Series<cheetah::Cpu, ValueT, Alloc>& Series<cheetah::Cpu, ValueT, Alloc>::operator=(Series const& copy)
{
    this->resize(copy.size());
    panda::copy(copy.begin(), copy.end(), this->begin());
    return *this;
}

template <typename ValueT, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<cheetah::Cpu, ValueT, Alloc>& Series<cheetah::Cpu, ValueT, Alloc>::operator=(Series<OtherArch, OtherValueT, OtherAlloc> const& copy)
{
    this->resize(copy.size());
    panda::copy(copy.begin(), copy.end(), this->begin());
    return *this;
}

// --- specialisations for the data type traits helpers
template<class ArchT, typename ValueT, typename AllocatorT>
struct DataTypeTraits<Series<ArchT, ValueT, AllocatorT>>
{
    typedef Series<ArchT, ValueT, AllocatorT> type;
    typedef ArchT Architecture;
    typedef ValueT value_type;
    typedef AllocatorT Allocator;
};

} // namespace data
} // namespace cheetah
} // namespace ska
