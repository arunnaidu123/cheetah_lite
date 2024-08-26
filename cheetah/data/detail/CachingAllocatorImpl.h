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
#ifndef SKA_CHEETAH_DATA_CACHINGALLOCATORIMPL_H
#define SKA_CHEETAH_DATA_CACHINGALLOCATORIMPL_H

#include "cheetah/data/detail/CachingAllocatorImplBase.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief      Prototype for specific implementation of Caching Allocator
 *
 * @tparam     Arch  The architecture
 * @tparam     T     The value type
 */
template <typename Arch, typename T>
class CachingAllocatorImpl
{
};

/**
 * @brief      A CPU-based CachingAllocator implementation
 *
 * @tparam     T     The value type
 */
template <typename T>
class CachingAllocatorImpl<cheetah::Cpu,T>: public CachingAllocatorImplBase<CachingAllocatorImpl<cheetah::Cpu,T>,T>
{
    private:
        //friend the base class to access the cached blocks
        friend class CachingAllocatorImplBase<CachingAllocatorImpl<cheetah::Cpu,T>,T>;

        //Directly allocate cpu memory
        static T* _allocate(std::size_t num_elements);

        //Directly free cpu memory
        static void _free(T* ptr);
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/cuda/detail/CachingAllocatorImpl.cuh"
#include "cheetah/data/detail/CachingAllocatorImpl.cpp"

#endif // SKA_CHEETAH_DATA_CACHINGALLOCATORIMPL_H
