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
#ifndef SKA_CHEETAH_DATA_CACHINGALLOCATORIMPLBASE_H
#define SKA_CHEETAH_DATA_CACHINGALLOCATORIMPLBASE_H

#include <mutex>
#include <map>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief      Base class for CachingAllocator implementations.
 *
 * @tparam     DerivedType  { description }
 * @tparam     T            { description }
 */
template <typename DerivedType, typename T>
class CachingAllocatorImplBase
{
    protected:
        typedef std::multimap<std::size_t, T*> FreeBlocksType;
        typedef std::map<T*, std::size_t>      AllocatedBlocksType;

    public:
        /**
         * @brief      Create a new instance
         */
        CachingAllocatorImplBase();
        ~CachingAllocatorImplBase();

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

    protected:
        FreeBlocksType      _free_blocks;
        AllocatedBlocksType  _allocated_blocks;
        std::mutex _mutex;
        void free_all();
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/CachingAllocatorImplBase.cpp"

#endif // SKA_CHEETAH_DATA_CACHINGALLOCATORIMPLBASE_H
