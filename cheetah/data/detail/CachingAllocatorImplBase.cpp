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
#include "cheetah/data/detail/CachingAllocatorImplBase.h"
#include "panda/Log.h"
#include "panda/Error.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename DerivedType, typename T>
CachingAllocatorImplBase<DerivedType,T>::CachingAllocatorImplBase()
{
}

template <typename DerivedType, typename T>
CachingAllocatorImplBase<DerivedType, T>::~CachingAllocatorImplBase()
{
    // free all allocations when CachingAllocatorImplBase goes out of scope
    free_all();
}

template <typename DerivedType, typename T>
T* CachingAllocatorImplBase<DerivedType,T>::allocate(std::size_t num_elements)
{
    std::lock_guard<std::mutex> lock(_mutex);
    T* result = nullptr;

    // search the cache for a free block
    typename FreeBlocksType::iterator free_block = _free_blocks.find(num_elements);

    if(free_block != _free_blocks.end())
    {
        // get the pointer
        result = free_block->second;

        // erase from the _free_blocks map
        _free_blocks.erase(free_block);
    }
    else
    {
        // no allocation of the right size exists
        // create a new one with cuda::malloc
        // throw if cuda::malloc can't satisfy the request
        try
        {
            // allocate memory and convert cuda::pointer to raw pointer
            result = DerivedType::_allocate(num_elements);
        }
        catch(std::runtime_error &e)
        {
            throw panda::Error(e);
        }
    }

    // insert the allocated pointer into the _allocated_blocks map
    _allocated_blocks.insert(std::make_pair(result, num_elements));
    return result;
}

template <typename DerivedType, typename T>
void CachingAllocatorImplBase<DerivedType,T>::deallocate(T* ptr, std::size_t /*n*/)
{
    std::lock_guard<std::mutex> lock(_mutex);
    // erase the allocated block from the allocated blocks map
    typename AllocatedBlocksType::iterator iter = _allocated_blocks.find(ptr);
    std::size_t num_elements = iter->second;
    _allocated_blocks.erase(iter);

    // insert the block into the free blocks map
    _free_blocks.insert(std::make_pair(num_elements, ptr));
}

template <typename DerivedType, typename T>
void CachingAllocatorImplBase<DerivedType,T>::free_all()
{
    std::lock_guard<std::mutex> lock(_mutex);
    // deallocate all outstanding blocks in both lists
    for(typename FreeBlocksType::iterator i = _free_blocks.begin();
        i != _free_blocks.end();
        ++i)
    {
        // transform the pointer to cuda::pointer before calling cuda::free
        DerivedType::_free(i->second);
    }

    for(typename AllocatedBlocksType::iterator i = _allocated_blocks.begin();
        i != _allocated_blocks.end();
        ++i)
    {
        // transform the pointer to cuda::pointer before calling cuda::free
        DerivedType::_free(i->first);
    }
}

} // namespace data
} // namespace cheetah
} // namespace ska
