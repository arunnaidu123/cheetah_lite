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
#include "cheetah/data/CachingAllocator.h"
#include "panda/Log.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename Arch, typename T>
CachingAllocator<Arch,T>::CachingAllocator()
    : _pimpl(std::unique_ptr<ImplType>(new ImplType()))
{
}

template <typename Arch, typename T>
CachingAllocator<Arch,T>::~CachingAllocator()
{
}

template <typename Arch, typename T>
T* CachingAllocator<Arch,T>::allocate(std::size_t num_elements)
{
    return _pimpl->allocate(num_elements);
}

template <typename Arch, typename T>
void CachingAllocator<Arch,T>::deallocate(T *ptr, std::size_t n)
{
    return _pimpl->deallocate(ptr,n);
}

} // namespace data
} // namespace cheetah
} // namespace ska
