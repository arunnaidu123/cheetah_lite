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

#ifndef SKA_CHEETAH_DATA_DATASEQUENCE2DPARTIALRANGE_H
#define SKA_CHEETAH_DATA_DATASEQUENCE2DPARTIALRANGE_H

#include "cheetah/data/DataSequence2D.h"
#include "cheetah/data/DataSequence2DPartialIterator.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename Derived>
struct DataSequence2DPartialRangeTraits;

template <typename Derived>
class DataSequence2DPartialRangeImpl
{
    using Traits = DataSequence2DPartialRangeTraits<Derived>;
    using DataType = typename Traits::DataType;
    using Iterator = typename Traits::Iterator;

    public:
        DataSequence2DPartialRangeImpl(
                DataType& data,
                std::size_t fbegin,
                std::size_t fend,
                std::size_t sbegin,
                std::size_t send,
                std::size_t flength);

        /**
         * @brief Returns the begin iterator
         */
        Iterator begin() const;

        /**
         * @brief Returns the end iterator
         */
        Iterator end() const;

    private:
        DataType& _data;
        std::size_t _fbegin, _fend;
        std::size_t _sbegin, _send;
        std::size_t _raw_flength;
};

template <typename Arch, typename Type>
class DataSequence2DPartialRange : public DataSequence2DPartialRangeImpl<DataSequence2DPartialRange<Arch,Type>>
{
    using BaseType = DataSequence2DPartialRangeImpl<DataSequence2DPartialRange<Arch,Type>>;
    using Traits = DataSequence2DPartialRangeTraits<DataSequence2DPartialRange<Arch,Type>>;

    public:
        using DataType = typename Traits::DataType;
        using Iterator = typename Traits::Iterator;
        using BaseType::BaseType;
};

template <typename Arch, typename Type>
class DataSequence2DConstPartialRange : public DataSequence2DPartialRangeImpl<DataSequence2DConstPartialRange<Arch,Type>>
{
    using BaseType = DataSequence2DPartialRangeImpl<DataSequence2DConstPartialRange<Arch,Type>>;
    using Traits = DataSequence2DPartialRangeTraits<DataSequence2DConstPartialRange<Arch,Type>>;

    public:
        using DataType = typename Traits::DataType;
        using Iterator = typename Traits::Iterator;
        using BaseType::BaseType;
};

template <typename Arch, typename Type>
struct DataSequence2DPartialRangeTraits<DataSequence2DPartialRange<Arch,Type>>
{
    using DataType = DataSequence2D<Arch,Type>;
    using Iterator = DataSequence2DPartialIterator<Arch, Type>;
};

template <typename Arch, typename Type>
struct DataSequence2DPartialRangeTraits<DataSequence2DConstPartialRange<Arch,Type>>
{
    using DataType = DataSequence2D<Arch,Type> const;
    using Iterator = DataSequence2DPartialIterator<Arch, Type const>;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DataSequence2DPartialRange.cpp"

#endif

