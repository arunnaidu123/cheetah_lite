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
#ifndef SKA_CHEETAH_DATA_DATASEQUENCE2DPARTIALITERATOR_H
#define SKA_CHEETAH_DATA_DATASEQUENCE2DPARTIALITERATOR_H

#include <iterator>
#include <memory>

#include "cheetah/data/DataSequence2D.h"
#include "cheetah/data/DataSequence2DFastIterator.h"
#include "cheetah/data/DataSequence2DIterator.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Iterator over partial ranges of DataSequence2D types, "fast" axis. Generic type.
 *
 */
template <typename Arch, typename Type>
class DataSequence2DPartialIterator : public DataSequence2DIterator<Arch,Type>
{
};


/**
 * @brief Iterator over partial ranges of DataSequence2D types, "fast" axis. CPU specialisation.
 *
 * @details Created by DataSequence2d::slice() or by constructor, iterates over a subset of the
 * DataSequence2D as if it were a FastIterator over a smaller datasequence.
 *
 * useful to do a partial copy of a slice, e.g.:
 * std::copy(ds2d.slice(10,20,10,20), ds2d.slice(10,20,10,20).end(), newds2d.begin());
 *
 * or copying a smaller ds2d into a larger one for buffering, e.g.:
 * std::copy(small.begin(),small.end(),
 * large.slice(currentpos, currentpos+small.fast_axis_length(), 0,small.slow_axis_length());
 * curentpos+=small.fast_axis_length();
 *
 * @tparam Type The datatype stored in the DataSequence2D.
 */
template <typename Type>
class DataSequence2DPartialIterator<Cpu,Type> : public DataSequence2DFastIteratorBase<Type, DataSequence2DPartialIterator<Cpu,Type> >
{
        typedef DataSequence2DFastIteratorBase<Type, DataSequence2DPartialIterator<Cpu,Type>> BaseT;

    public:
        typedef typename std::iterator_traits<DataSequence2DPartialIterator<Cpu,Type>>::pointer PtrType;
        typedef typename std::iterator_traits<DataSequence2DPartialIterator<Cpu,Type>>::reference RefType;
        typedef typename std::iterator_traits<DataSequence2DPartialIterator<Cpu,Type>>::difference_type DiffType;
        typedef DataSequence2DPartialIterator<Cpu,Type> SelfType;
        typedef SelfType Iterator;

    public:
        /**
         * @brief Create an iterator
         *
         * @details Iterator starts at the top of the "slow" axis. Iterates
         * only in the range fbegin->fend, and sbegin->send
         *
         * @arg data_ptr the data to iterate over
         * @arg fbegin the start of the partial region in fast axis
         * @arg fend the end of the partial region in fast axis
         * @arg sbegin the start of the partial region in slow axis
         * @arg send the end of the partial region in slow axis
         */
        DataSequence2DPartialIterator(const DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data_ptr,
                std::size_t fbegin, std::size_t fend, std::size_t sbegin, std::size_t send);

        DataSequence2DPartialIterator(DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data_ptr,
                std::size_t fbegin, std::size_t fend, std::size_t sbegin, std::size_t send);

        /**
         * @brief default constructor. Required by random_access_iterator_tag
         */
        DataSequence2DPartialIterator();

        /**
         * @brief copy constructor. Required by random_access_iterator_tag.
         */
        DataSequence2DPartialIterator(const DataSequence2DPartialIterator<Cpu,Type> &copy);

        /**
         * @brief Destroy the iterator
         */
        ~DataSequence2DPartialIterator();

        /**
         * @brief dereference operator
         * @details return the actual data currently referenced by the iterator
         */
        PtrType operator->() const override;

        /**
         * @brief dereference operator
         */
        RefType operator*() const override;

        /**
         * Return an iterator to the start of the partial region.
         */
        SelfType begin() const;

        /**
         * Return an iterator pointing at the end of the partial region.
         */
        SelfType end() const;

    protected:
        std::size_t mangle_offset(std::size_t off) const;

    private:
        std::size_t _fbegin;
        std::size_t _fend;
        std::size_t _sbegin;
        std::size_t _send;
        std::size_t _flength;

};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DataSequence2DPartialIterator.cpp"
#include "cheetah/data/detail/DataSequence2DPartialIterator_bool.cpp"

#endif // SKA_CHEETAH_DATA_DATASEQUENCE2DPARTIALITERATOR_H
