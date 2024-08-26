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
#ifndef SKA_CHEETAH_DATA_DATASEQUENCE2D_H
#define SKA_CHEETAH_DATA_DATASEQUENCE2D_H

#include <cstddef>
#include <vector>
#include <panda/DataChunk.h>

#include "cheetah/utils/Architectures.h"
#include "cheetah/data/DataSequence2DIterator.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A 2-dimensional sequencial data of type T.
 *
 * @details This generic type should never be instantiated.
 * @tparam Arch is a tag for the type of memory used
 * @tparam T is the underlying datatype.
 */
template <typename Arch, typename T>
class DataSequence2D
{
};

}
}
}

#include "cheetah/data/DataSequence2DFastIterator.h"
#include "cheetah/data/DataSequence2DPartialIterator.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A CPU implementation of the DataSequence2D
 *
 * @details A two dimensional data structure, guaraneteed to be
 * stored in continuous memory. Has a "fast" and "slow" axis.
 * The "fast" axis is the index that moves fastest as one
 * moves through the memory, and the "slow" axis moves slowest
 * as one moves through the memory. Typically the "fast" axis
 * is equivilent to "columns" and the slow axis is "rows"
 *
 * The index to the memory is such that
 * memory index = FastAxis + fast_axis_length*SlowAxis
 *
 * Allows access to a pointer of type T*, but may be backed
 * by any data structure.
 *
 * @tparam T is the type of the pointer stored.
 */
template <typename T>
class DataSequence2D<Cpu,T> {
    public:
        typedef T DataType;
        typedef DataSequence2DFastIterator<Cpu,DataType> Iterator;
        typedef DataSequence2DFastIterator<Cpu,const DataType> ConstIterator;
        typedef DataSequence2DPartialIterator<Cpu,DataType> PartialIterator;

    public:

        /**
         * @brief Create a zero-sized DataSequence2D
         */
        DataSequence2D();

        /**
         * @brief Create a pre-sized DataSequence2D filled with the
         * default constructor of T.
         *
         * @param fast_axis_length the length of the fast axis
         * @param slow_axis_length the length of the slow axis.
         */
        DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length);

        /**
         * @brief Create a pre-sized DataSequence2D, filled with
         * copies of a provided T.
         *
         * @param fast_axis_length the length of the fast axis
         * @param slow_axis_length the length of the slow axis.
         * @param fill object to be copied.
         */
        DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length, const T &fill);

        /**
         * @brief Free memory associated with DataSequence2D
         */
        ~DataSequence2D();

        /**
         * @brief Get the current fast_axis_length
         */
        inline std::size_t fast_axis_length() const;

        /**
         * @brief Get the current slow_axis_length
         */
        inline std::size_t slow_axis_length() const;

        /**
         * @brief the number fo elements in the data
         */
        inline std::size_t size() const;

        /**
         * @brief Destructive resize of underlying data. Fill with copies.
         *
         * @param fast_axis_length new fast axis length
         * @param slow_axis_length new slow axis length
         * @param fill Value to copy to fill underlying array.
         */
        void resize(std::size_t fast_axis_length, std::size_t slow_axis_length, const T &fill);

        /**
         * @brief Destructive resize of underlying data.
         *
         * @details the contents of the memory are not defined after
         * a call to this method.
         *
         * @param fast_axis_length new fast axis length
         * @param slow_axis_length new slow axis length
         * @param fill Value to copy to fill underlying array.
         */
        void resize(std::size_t fast_axis_length, std::size_t slow_axis_length);

        /**
         * @brief access a read-only pointer to the data
         */
        T const* data() const;

        /**
         * @brief access a pointer to the data
         */

        T* data();

        /**
         * @brief get the default Iterator
         * @details iterates over fast axis first, then slow axis
         */
        Iterator begin();

        /**
         * @brief get the default Iterator
         * @details iterates over fast axis first, then slow axis
         */
        ConstIterator begin() const;

        /**
         * @brief get the default Iterator
         * @details iterates over fast axis first, then slow axis
         */
        ConstIterator cbegin() const;

        /**
         * @brief get the default end Iterator
         * @details iterates over fast axis first, then slow axis
         */
        Iterator end();

        /**
         * @brief get the default end Iterator
         * @details iterates over fast axis first, then slow axis
         */
        ConstIterator end() const;

        /**
         * @brief get the default end Iterator
         * @details iterates over fast axis first, then slow axis
         */
        ConstIterator cend() const;

        /**
         * @brief Return an Iterator to a slice of the datasequence.
         * @details The partial Iterator returned will iterate only over the
         * range specified, from fbegin to fend on the fast axis and from
         * sbegin to send on the slow axis. Otherwise the Iterator behaves
         * as if it were a FastIterator over a smaller DataSequence2D of length
         * (fend-fbegin) by (send-sbegin)
         * @arg fbegin the fast axis to start at (inclusive)
         * @arg fend the fast axis to end at (not inclusive)
         * @arg sbegin the slow axis to start at (inclusive)
         * @arg send the slow axis to end at (not inclusive)
         */
        PartialIterator slice(
                std::size_t fbegin,
                std::size_t fend,
                std::size_t sbegin,
                std::size_t send);

    private:
        std::size_t _fast_axis_length = 0;
        std::size_t _slow_axis_length = 0;
        std::vector<T> _data;
};

template<typename Arch, typename DataType>
bool operator==(DataSequence2D<Arch, DataType> const&, DataSequence2D<Arch, DataType> const&);

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DataSequence2D.cpp"
#include "cheetah/data/detail/DataSequence2D_bool.cpp"

#endif // SKA_CHEETAH_DATA_DATASEQUENCE2D_H
