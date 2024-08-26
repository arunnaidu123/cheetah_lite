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

#include "panda/Error.h"
#include "cheetah/data/DataSequence2DPartialRange.h"

namespace ska {
namespace cheetah {
namespace data {

template <>
class DataSequence2D<Cpu,bool> {
    public:
        typedef bool DataType;
        typedef std::vector<bool>::iterator Iterator;
        typedef std::vector<bool>::const_iterator ConstIterator;

        typedef DataSequence2DPartialRange<Cpu,bool> PartialRange;
        typedef DataSequence2DConstPartialRange<Cpu,bool> ConstPartialRange;

        typedef DataSequence2DPartialIterator<Cpu,bool> PartialIterator;
        typedef DataSequence2DPartialIterator<Cpu,const bool> ConstPartialIterator;

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
        DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length, const bool &fill);

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
        void resize(std::size_t fast_axis_length, std::size_t slow_axis_length, const bool &fill);

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
        PartialRange slice(
                std::size_t fbegin,
                std::size_t fend,
                std::size_t sbegin,
                std::size_t send);

        ConstPartialRange slice(
                std::size_t fbegin,
                std::size_t fend,
                std::size_t sbegin,
                std::size_t send) const;

    private:
        std::size_t _fast_axis_length = 0;
        std::size_t _slow_axis_length = 0;
        std::vector<bool> _data;
};

std::size_t DataSequence2D<Cpu,bool>::fast_axis_length() const { return this->_fast_axis_length;}

std::size_t DataSequence2D<Cpu,bool>::slow_axis_length() const { return this->_slow_axis_length;}

std::size_t DataSequence2D<Cpu,bool>::size() const { return this->_data.size();}

} // namespace data
} // namespace cheetah
} // namespace ska



