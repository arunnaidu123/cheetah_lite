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
#ifndef SKA_CHEETAH_DATA_SERIES_H
#define SKA_CHEETAH_DATA_SERIES_H

#include "cheetah/Configuration.h"

#include "cheetah/data/VectorLike.h"
#include "cheetah/data/DefaultAllocator.h"
#include "cheetah/utils/Architectures.h"
#include "panda/Device.h"
#include "panda/DeviceMemory.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Base class for generic data series
 *
 * @details A data series class for storage of arbitrary data.
 *
 */
template <typename Arch, typename ValueT, typename Alloc=typename data::DefaultAllocator<Arch, ValueT>::type>
class Series
{
        typedef panda::DeviceMemory<Arch, ValueT, Alloc> DeviceMemoryType;

    public:
        typedef typename DeviceMemoryType::Iterator Iterator;
        typedef typename DeviceMemoryType::ConstIterator ConstIterator;
        typedef Alloc Allocator;
        typedef Arch Architecture;
        typedef ValueT ValueType;
        typedef ValueT value_type;

    public:
        Series(Allocator const&);
        Series(std::size_t size, Allocator const&);
        Series(Series const& copy);
        Series(Series&& copy_to_move);

        /**
         * @brief copies Series data to the device
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator);

        /**
         * @brief assignment operator
         */
        Series& operator=(Series const&);

        /**
         * @brief assignment operator
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series& operator=(Series<OtherArch, OtherValueT, OtherAlloc> const&);

        /**
         * @brief Iterators to device memory
         * @details note that this memory is not directly accesible to the host
         *          to use on the host convert to Series<Cpu, ValueType> which
         *          will copy the data to the host
         */
        ConstIterator begin() const;
        Iterator begin();
        Iterator end();
        ConstIterator end() const;

        ConstIterator cbegin() const;
        ConstIterator cend() const;

        /**
         * @brief the size of the series
         */
        std::size_t size() const;

        /**
         * @brief resize the data
         */
        void resize(std::size_t size);

    private:
        DeviceMemoryType _data;
};

/**
 * @brief specialisation Series for Cpu
 */
template <typename ValueType, typename Alloc>
class Series<cheetah::Cpu, ValueType, Alloc>: public VectorLike<std::vector<ValueType,Alloc>>
{
        typedef VectorLike<std::vector<ValueType,Alloc>> BaseT;

    public:
        typedef cheetah::Cpu Architecture;
        typedef Alloc Allocator;
        typedef ValueType value_type;

    public:
        Series();
        Series(Allocator const&);
        Series(std::size_t size, Allocator const&);
        Series(Series const&);

        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy);

        /**
         * @brief copy constructor. Series data transfered from Series data on a device
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator);

        /**
         * @brief assignment operator
         */
        Series& operator=(Series const&);

        /**
         * @brief assignment operator
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series& operator=(Series<OtherArch, OtherValueT, OtherAlloc> const&);
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "detail/Series.cpp"
#include "cheetah/data/cuda/Series.h"
#include "cheetah/data/altera/Series.h"

#endif // SKA_CHEETAH_DATA_SERIES_H
