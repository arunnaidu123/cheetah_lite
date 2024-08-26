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
#ifndef SKA_CHEETAH_DATA_CACHINGALLOCATOR_H
#define SKA_CHEETAH_DATA_CACHINGALLOCATOR_H

#include "cheetah/data/detail/CachingAllocatorImpl.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief     A generic caching allocator implementation
 *
 * @details   Mainly used by thrust to provide memory required by
 *            certain transforms (e.g. copy_if). This class provides
 *            a map between allocated memory of a given size and type
 *            and pointers to that memory. It is best used for systems
 *            that regularly allocate and deallocate the same amount of
 *            memory (again e.g. copy_if and other stream compaction
 *            systems). Should not be used for systems the require
 *            allocation and deallocation of many different random
 *            memory sizes.
 *
 *            This class uses CRTP to support implementation on multiple
 *            architectures.
 *
 * @tparam     DerivedType  The derived type that is inheriting from this class
 * @tparam     T            The value type of the allocator
 *
 */
template <typename Arch, typename T>
class CachingAllocator
{
    public:
        typedef T value_type;

    private:
        typedef CachingAllocatorImpl<Arch,T> ImplType;

    public:
        /**
         * @brief      Create a new instance
         */
        CachingAllocator();
        CachingAllocator(CachingAllocator const&) = delete;
        CachingAllocator(CachingAllocator&&) = default;
        ~CachingAllocator();

        /**
         * @brief      Allocate memory
         *
         * @param[in]  num_elements  The number elements to be allocate of type T
         *
         * @return     A pointer to the allocated memory
         */
        T* allocate(std::size_t num_elements);

        /**
         * @brief      Deallocate memory
         *
         * @param[in]  ptr   Pointer to the memory to be deallocated
         * @param[in]  n     The number of elements of type T to be deallocated
         */
        void deallocate(T *ptr, std::size_t /*n*/);

    private:
        std::unique_ptr<ImplType> _pimpl;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/CachingAllocator.cpp"

#endif // SKA_CHEETAH_DATA_CACHINGALLOCATOR_H
