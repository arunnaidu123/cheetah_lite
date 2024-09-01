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

#ifndef SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFERFILLERIMPL_H
#define SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFERFILLERIMPL_H

#include "panda/TypeTraits.h"
#include "panda/Buffer.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename DataType>
struct HasBeginT
{
    template<typename T>
    using TesterT = decltype(std::declval<T>().begin(std::declval<DataType&>()));
};

template<typename Traits, typename DataType>
struct has_begin : HasMethod<Traits, HasBeginT<DataType>::template TesterT>
{
};

template<typename DataType>
struct HasEndT
{
    template<typename T>
    using TesterT = decltype(std::declval<T>().end(std::declval<DataType&>()));
};

template<typename Traits, typename DataType>
struct has_end : HasMethod<Traits, HasEndT<DataType>::template TesterT>
{};

template<typename T>
using HasSizeTesterT = decltype(std::declval<T const&>().size());

template<typename Traits>
struct has_size : HasMethod<Traits, HasSizeTesterT>
{};


template<typename T>
using HasResizeTesterT = decltype(std::declval<T&>().resize(std::declval<decltype(std::declval<T const&>().size())>()));

template<typename Traits>
struct has_resize : HasMethod<Traits, HasResizeTesterT>
{};

template<typename T>
using HasNumberOfElements = decltype(std::declval<T&>().number_of_elements(std::declval<HasSizeTesterT<T>>()));

template<typename Traits>
struct has_number_of_elements : HasMethod<Traits, HasNumberOfElements>
{};

template<typename T>
using HasCreateBufferT = decltype(std::declval<T>().create_buffer());
template<typename Traits>
struct has_create_buffer : HasMethod<Traits, HasCreateBufferT>
{};

// traits helpers
template<typename DataType, typename Traits, typename Enable=void>
struct TraitsEndHelper : public Traits
{
    private:
        typedef Traits BaseT;

    public:
        template<typename... Args>
        TraitsEndHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}
};

template<typename DataType, typename Traits>
struct TraitsEndHelper<DataType, Traits, typename std::enable_if<!has_end<Traits, DataType>::value>::type>
     : public Traits
{
    private:
        typedef Traits BaseT;

    public:
        template<typename... Args>
        TraitsEndHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}

        auto end(DataType const& data) -> decltype(std::declval<DataType const&>().end())
        {
            return data.end();
        }
};

template<typename DataType, typename Traits, typename Enable=void>
struct TraitsBeginHelper
    : public TraitsEndHelper<DataType, Traits>
{
    private:
        typedef TraitsEndHelper<DataType, Traits> BaseT;

    public:

        template<typename... Args>
        TraitsBeginHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}
};

template<typename DataType, typename Traits>
struct TraitsBeginHelper<DataType, Traits, typename std::enable_if<!has_begin<Traits, DataType>::value>::type>
    : public TraitsEndHelper<DataType, Traits>
{
    private:
        typedef TraitsEndHelper<DataType, Traits> BaseT;

    public:

        template<typename... Args>
        TraitsBeginHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}

        auto begin(DataType const& data) -> decltype(std::declval<DataType const&>().begin())
        {
            return data.begin();
        }
};

template<typename DataType, typename Traits, typename Enable=void>
class TraitsBufferSizeHelper
    : public TraitsBeginHelper<DataType, Traits>
{
    private:
        typedef TraitsBeginHelper<DataType, Traits> BaseT;

    public:
        template<typename... Args>
        TraitsBufferSizeHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
    {}
};

template<typename DataType, typename Traits>
class TraitsBufferSizeHelper<DataType, Traits, typename std::enable_if<!has_size<Traits>::value
                                                                   || !has_resize<Traits>::value
                                                                   || !has_number_of_elements<Traits>::value
                                                                  >::type>
    : public TraitsBeginHelper<DataType, Traits>
{
    private:
        typedef TraitsBeginHelper<DataType, Traits> BaseT;
        typedef decltype(std::declval<BaseT>().begin(std::declval<const DataType>())) DataIteratorT;
        typedef typename std::iterator_traits<DataIteratorT>::value_type value_type;

    public:
        template<typename... Args>
        TraitsBufferSizeHelper(std::size_t size, Args&&... args)
            : BaseT(std::forward<Args>(args)...)
            , _size(size)
        {}


        void resize(std::size_t size)
        {
            _size = size;
        }

        std::size_t size() const
        {
            return _size;
        }

        std::size_t number_of_elements(std::size_t size) const
        {
            return size;
        }

    private:
        std::size_t _size;
};

template<typename DataType, typename Traits, typename Enable=void>
class TraitsBufferHelper
    : public TraitsBufferSizeHelper<DataType, Traits>
{
    private:
        typedef TraitsBufferSizeHelper<DataType, Traits> BaseT;

    public:
        template<typename... Args>
        TraitsBufferHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
    {}
};

template<typename DataType, typename Traits>
class TraitsBufferHelper<DataType, Traits, typename std::enable_if<!has_create_buffer<Traits>::value>::type>
    : public TraitsBufferSizeHelper<DataType, Traits>
{
        typedef TraitsBufferSizeHelper<DataType, Traits> BaseT;
        typedef decltype(std::declval<BaseT>().begin(std::declval<const DataType>())) DataIteratorT;
        typedef typename std::iterator_traits<DataIteratorT>::value_type value_type;

    public:
        template<typename... Args>
        TraitsBufferHelper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}

        panda::Buffer<value_type> create_buffer()
        {
            return panda::Buffer<value_type>(BaseT::size());
        }
};

template<typename DataType, typename TraitsT>
class AggregationBufferFillerTraitsAdaptor
    : public TraitsBufferHelper<DataType, TraitsT>
{
    typedef TraitsBufferHelper<DataType, TraitsT> BaseT;
    public:
        template<typename... Args>
        AggregationBufferFillerTraitsAdaptor(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFERFILLERIMPL_H
