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
#ifndef SKA_CHEETAH_DATA_VECTORLIKE_H
#define SKA_CHEETAH_DATA_VECTORLIKE_H

#include <cstddef>
#include <type_traits>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief      Class that wraps objects that export the std::vector interface
 *
 * @details    This class is used to provide interoperability between thrust vectors
 *             and std::vectors such that both can be used as template arguments for
 *             the Series class (from which TimeSeries, FrequencySeries, etc. are derived)
 *
 * @tparam     VectorType  The underlying vector type to be wrapped
 */
template <typename VectorType>
class VectorLike
{
        typedef VectorLike<VectorType> SelfType;

    public:
        typedef typename VectorType::iterator Iterator;
        typedef typename VectorType::const_iterator ConstIterator;
        typedef typename VectorType::reverse_iterator ReverseIterator;
        typedef typename VectorType::pointer Pointer;
        typedef typename VectorType::const_pointer ConstPointer;
        typedef typename VectorType::reference Reference;
        typedef typename VectorType::const_reference ConstReference;
        typedef typename VectorType::value_type ValueType;
        typedef typename VectorType::allocator_type Allocator;

    public:
        /**
         * @brief      Construct a VectorLike instance
         */
        VectorLike();

        /**
         * @brief      Construct a VectorLike instance with an explicit allocator
         */
        explicit VectorLike(Allocator const& allocator);

        /**
         * @brief      Construct a VectorLike instance of a given size
         *
         * @param[in]  n     The size of the underlying vector.
         */
        template<typename... Args>
        explicit VectorLike(std::size_t n, Args&&... args);

        /**
         * @brief      Construct a filled VectorLike instance of a given size
         *
         * @param[in]  n      The size of the underlying vector.
         * @param[in]  value  The fill value
         */
        explicit VectorLike(std::size_t n, const ValueType& value, Allocator const& allocator=Allocator());
        ~VectorLike();

        /**
         * @brief      Retrieve the size of the underlying vector
         */
        std::size_t size() const;

        /**
         * @brief      the first emelment
         */
        ValueType const& front() const;
        ValueType& front();

        /**
         * @brief      the last emelment
         */
        ValueType const& back() const;
        ValueType& back();

        /**
         * @brief      Resize the vector
         *
         * @param[in]  size  The new size of the vector
         */
        void resize(std::size_t size, const ValueType &x = ValueType());

        /**
         * @brief      Subscript access to the data contained in the underlying vector.
         *
         * @param[in]  n     The index of the elemment to be accessed.
         *
         * @return     Read/write reference to the data.
         */
        Reference operator[](std::size_t n);

        /**
         * @brief      Subscript Read-only access to the data contained in the underlying vector.
         *
         * @param[in]  n     The index of the elemment to be accessed.
         *
         * @return     Read reference to the data.
         */
        ConstReference operator[](std::size_t n) const;

        /**
         * @brief      An iterator pointing to the start of the vector.
         *
         * @return     A non-const iterator.
         */
        Iterator begin();

        /**
         * @brief      A constant iterator pointing to the start of the vector.
         *
         * @return     A const iterator.
         */
        ConstIterator begin() const;
        ConstIterator cbegin() const;

        /**
         * @brief      A reverse iterator pointing
         *             to the end of the vector
         *             (i.e the last element).
         */
        ReverseIterator rbegin();

        /**
         * @brief      An iterator pointing to the end of the vector.
         *
         * @return     A non-const iterator.
         */
        Iterator end();

        /**
         * @brief      A constant iterator pointing to the end of the vector.
         *
         * @return     A const iterator.
         */
        ConstIterator end() const;

        /**
         * @brief      A constant iterator pointing to the end of the vector.
         *
         * @return     A const iterator.
         */
        ConstIterator cend() const;

        /**
         * @ brief     A reverse iterator pointing to
         *             the beginning of the vector
         */
        ReverseIterator rend();

        /**
         * @brief      Return a pointer pointing to the start of the vector
         *
         * @return     A pointer to the start of the vector
         */
        Pointer data();

        /**
         * @brief      Return a constant pointer pointing to the start of the vector
         *
         * @return     A const pointer to the start of the vector
         */
        ConstPointer data() const;

        /**
         * @brief      Appends element to end of vector
         *
         * @param[in]  value     element to add to end of vector
         */
        void push_back(ValueType const& value);

        /**
         * @brief      Appends element to end of vector using the move operator
         *
         * @param[in]  value     element to add to end of vector
         */
        template<typename ...Args>
        void emplace_back(Args&&... value);

        /**
         * @brief      Reserve space for this many elements
         *
         * @param[in]  size  The number of elements
         */
        void reserve(std::size_t size);

        /**
         * @brief      The reseved size of the vector
         *
         * @return     The number of elements reserved
         */
        std::size_t capacity() const;

        /**
         * @brief       swaps the contents of this vector_base with another vector
         *
         * @param      v     The vector with which to swap.
         */
        void swap(VectorLike &v);

        /**
         * @brief       erase the element from a given position or a range of positions
         */
        Iterator erase(Iterator pos);

        Iterator erase(Iterator first, Iterator last);

        /**
         * @brief       insert a value in the vector_base
         */
        Iterator insert(Iterator pos, const ValueType& value);

        ConstIterator insert(ConstIterator pos, ValueType&& value);

        ConstIterator insert(ConstIterator pos, const ValueType& value);

        Iterator insert(Iterator pos, ValueType&& value);

        template<typename...Args>
        Iterator emplace(Iterator pos, Args&&...);

        template<typename...Args>
        ConstIterator emplace(ConstIterator pos, Args&&...);

        /**
         * @brief       add the contents of the argument to the current vector
         */
        SelfType& operator+=(SelfType const&);

        /**
         * @brief       return true if the vector is empty
         */
        bool empty() const;

        /**
         * @brief       clear the data
         */
        void clear();

        /**
         * @brief return the allocator
         */
        Allocator allocator() const;

    private:
        VectorType _data;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/VectorLike.cpp"

#endif // SKA_CHEETAH_DATA_VECTORLIKE_H
