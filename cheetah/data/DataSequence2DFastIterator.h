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
#ifndef SKA_CHEETAH_DATA_DATASEQUENCE2DFASTITERATOR_H
#define SKA_CHEETAH_DATA_DATASEQUENCE2DFASTITERATOR_H

#include <iterator>
#include <memory>

#include "cheetah/data/DataSequence2DIterator.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Iterator over DataSequence2D types, over "fast" axis. Generic type.
 * 
 */
template <typename Arch, typename Type>
class DataSequence2DFastIterator : public DataSequence2DIterator<Arch,Type>{
};

template<typename Arch, typename T>
class DataSequence2D;

/**
 * @brief Iterator over DataSequence2D types, over "fast" axis. CPU specific type.
 * 
 * @tparam Type The datatype stored in the DataSequence2D.
 */
template <typename Type, typename DerivedType>
class DataSequence2DFastIteratorBase : public std::iterator<std::random_access_iterator_tag, Type,std::size_t,Type*,Type&>
{
        typedef DataSequence2DFastIteratorBase<Type, DerivedType> SelfType;

    public:
        typedef typename std::iterator_traits<SelfType>::pointer PtrType;
        typedef typename std::iterator_traits<SelfType>::reference RefType;
        typedef typename std::iterator_traits<SelfType>::difference_type DiffType;

    public:
        /**
         * @brief Create an iterator
         *
         * @details Iterator starts at the top of the "slow" axis given by
         * slowOffset.
         * @arg data_ptr the data to iterate over
         * @arg slowOffset the slow axis offset, defaults to zero.
         */
        explicit DataSequence2DFastIteratorBase(const DataSequence2D<Cpu, typename std::remove_cv<Type>::type> &data_ptr,
                std::size_t slow_offset=0);

        explicit DataSequence2DFastIteratorBase(DataSequence2D<Cpu, typename std::remove_cv<Type>::type> &data_ptr,
                std::size_t slow_offset=0);

        /**
         * @brief default constructor. Required by random_access_iterator_tag
         */
        DataSequence2DFastIteratorBase();

        /**
         * @brief copy constructor. Required by random_access_iterator_tag.
         */
        DataSequence2DFastIteratorBase(const DataSequence2DFastIteratorBase<Type, DerivedType> &copy);

        /**
         * @brief Destroy the iterator
         */
        ~DataSequence2DFastIteratorBase();


        /**
         * @brief dereference operators
         * @details return the actual data currently referenced by the iterator
         */
        virtual PtrType operator->() const;

        /**
         * @brief dereference operators
         */
        virtual RefType operator*() const;


        /**
         * @brief pre-increment operator. 
         * @returns the iterator now pointing to the next element in the data sequence
         * @details at the end of the sequence will contain nullptr. 
         *          Unlike the pot-increment operator, no copy is made.
         */
        DerivedType& operator++();

        /**
         * @brief post-increment the iterator to the next element in the data sequence
         * @returns A copy of the iterator in its pre-incremented state.
         */
        DerivedType operator++(int);

        /**
         * @brief Decriment
         */
        DerivedType& operator--();

        /**
         * @brief Decriment
         */
        DerivedType operator--(int);

        /**
         * @brief Add-Asignment
         */
        DerivedType& operator+=(DiffType off);

        /**
         * @brief Subtract-asignment
         */
        DerivedType& operator-=(DiffType off);

        /**
         * Add offset
         */
        DerivedType operator+(DiffType off) const;

        /**
         * Subtract offset
         */
        DerivedType operator-(DiffType off) const;

        /**
         * Subtract iterator
         */
        DiffType operator-(const SelfType &right) const;

        /**
         * less than
         */
        bool operator<(const SelfType &right) const;

        /**
         * Greater Than
         */
        bool operator>(const SelfType &right) const;

        /**
         * Less than or Equal.
         */
        bool operator<=(const SelfType &right) const;

        /**
         * Greater or Equal
         */
        bool operator>=(const SelfType &right) const;

        /**
         * @brief Offset dereference
         */
        RefType operator[](DiffType off) const;

        /**
         * @brief return true if it is the same data and offset
         */
        bool operator==(SelfType const&) const;

        /**
         * @brief return true if it has neither the same data or offset
         */
        bool operator!=(SelfType const&) const;

    protected:
        std::size_t index();
        const DataSequence2D<Cpu,typename std::remove_cv<Type>::type> *_data;
        std::size_t _offset=0;
        Type* _raw;

};

template <typename Type>
class DataSequence2DFastIterator<Cpu,Type> : public DataSequence2DFastIteratorBase<Type, DataSequence2DFastIterator<Cpu, Type>>
{
    typedef DataSequence2DFastIteratorBase<Type, DataSequence2DFastIterator<Cpu, Type>> BaseT;
    
    public:
        using BaseT::BaseT;

};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DataSequence2DFastIterator.cpp"

#endif // SKA_CHEETAH_DATA_DATASEQUENCE2DFASTITERATOR_H 
