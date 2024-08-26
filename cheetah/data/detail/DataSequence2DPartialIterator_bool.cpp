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

#include "cheetah/data/DataSequence2DPartialIterator.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace data {

template <typename RawIterator>
class DataSequence2DPartialIteratorImpl
      : public std::iterator<std::forward_iterator_tag, bool>
{
    using SelfType = DataSequence2DPartialIteratorImpl;
    using BaseType = RawIterator;

    public:
        //typedef typename BaseType::iterator_category iterator_category;
        typedef typename std::iterator_traits<RawIterator>::difference_type difference_type;

    public:
        /**
         * @brief default constructor. Required by random_access_iterator_tag
         */
        DataSequence2DPartialIteratorImpl()
            = default;

        /**
         * @brief copy constructor. Required by random_access_iterator_tag.
         */
        DataSequence2DPartialIteratorImpl(
                const DataSequence2DPartialIteratorImpl &copy)
            = default;

        /**
         * @brief construct from an iterator.
         */
        DataSequence2DPartialIteratorImpl(
                BaseType const &iterator)
            : _it(iterator)
            , _fast_index(0)
        {}

        /**
         * @brief construct from an iterator and dimension sizes.
         */
        DataSequence2DPartialIteratorImpl(
                BaseType const &iterator,
                std::size_t fast_length,
                std::size_t slow_stride)
            : _it(iterator)
            , _fast_index(0)
            , _slow_stride(slow_stride)
            , _fast_length(fast_length)
        {}

        /**
         * @brief Destroy the iterator
         */
        ~DataSequence2DPartialIteratorImpl() {}

        bool operator==(SelfType const& o) const
        {
            return _it == o._it;
        }

        bool operator!=(SelfType const& o) const
        {
            return _it != o._it;
        }

        typename BaseType::reference operator*() 
        {
            return *_it;
        }

        typename BaseType::reference const operator*() const
        {
            return *_it;
        }

        SelfType& operator+=(typename BaseType::reference const v)
        {
            _it += v;
            return *this;
        }

        SelfType& operator++()
        {
            ++_it;

            if (++_fast_index < _fast_length) return *this;

            _it += _slow_stride;
            _fast_index = 0;
            return *this;
        }

        difference_type operator-(SelfType const o)
        {
            std::size_t diff = _fast_index + (o._fast_length - o._fast_index);
            return (_it - o._it)/(o._fast_length + o._slow_stride) + diff - 1;
        }

        RawIterator& raw_it()
        {
            return _it;
        }

        RawIterator const& raw_it() const
        {
            return _it;
        }

    private:
        RawIterator _it;
        std::size_t _fast_index;
        std::size_t _slow_stride;
        std::size_t _fast_length;
};

template <>
class DataSequence2DPartialIterator<Cpu, bool>
    : public DataSequence2DPartialIteratorImpl<std::vector<bool>::iterator>
{
    using BaseType = DataSequence2DPartialIteratorImpl<std::vector<bool>::iterator>;

    public:
        using BaseType::BaseType;
};

template <>
class DataSequence2DPartialIterator<Cpu, const bool>:
    public DataSequence2DPartialIteratorImpl<std::vector<bool>::const_iterator>
{
    using BaseType = DataSequence2DPartialIteratorImpl<std::vector<bool>::const_iterator>;

    public:
        using BaseType::BaseType;
};
} // namespace data
} // namespace cheetah
} // namespace ska

#include <algorithm>

namespace std {
    template<typename Arch>
    struct iterator_traits<ska::cheetah::data::DataSequence2DPartialIterator<Arch, bool>>
    {
        typedef typename ska::cheetah::data::DataSequence2DPartialIterator<Arch, bool>::iterator_category iterator_category;
    };

    template<> inline 
    void fill<typename ska::cheetah::data::DataSequence2DPartialIterator<ska::cheetah::Cpu, bool>, bool>(typename ska::cheetah::data::DataSequence2DPartialIterator<ska::cheetah::Cpu, bool> begin, typename ska::cheetah::data::DataSequence2DPartialIterator<ska::cheetah::Cpu, bool> end, const bool& x)
    {
        std::fill(begin.raw_it(), end.raw_it(), x);
    }


    template< class Arch>
    typename std::iterator_traits<ska::cheetah::data::DataSequence2DPartialIterator<Arch,bool>>::difference_type
    distance( ska::cheetah::data::DataSequence2DPartialIterator<Arch,bool> const& first, ska::cheetah::data::DataSequence2DPartialIterator<Arch,bool> const& last )
    {
        return last - first;
    }

}
