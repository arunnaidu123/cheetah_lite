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
#include "cheetah/data/test_utils/CachingAllocatorTest.h"
#include "cheetah/data/CachingAllocator.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template <typename CachingAllocatorTestTraitsType>
CachingAllocatorTest<CachingAllocatorTestTraitsType>::CachingAllocatorTest()
    : ::testing::Test()
{
}

template <typename CachingAllocatorTestTraitsType>
CachingAllocatorTest<CachingAllocatorTestTraitsType>::~CachingAllocatorTest()
{
}


template <typename CachingAllocatorTestTraitsType>
void CachingAllocatorTest<CachingAllocatorTestTraitsType>::SetUp()
{
}

template <typename CachingAllocatorTestTraitsType>
void CachingAllocatorTest<CachingAllocatorTestTraitsType>::TearDown()
{
}

template <typename TypeParam>
void AllocateDeallocateTest<TypeParam>::test()
{
    CachingAllocator<typename TypeParam::Architecture, typename TypeParam::ValueType> cache;
    auto* a = cache.allocate(1000);
    auto* b = cache.allocate(20);
    cache.deallocate(a,1000);
    auto* c = cache.allocate(4444);
    cache.deallocate(b,20);
    cache.deallocate(c,4444);
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
