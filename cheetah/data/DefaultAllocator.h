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
#ifndef SKA_CHEETAH_DATA_DEFAULTALLOCATOR_H
#define SKA_CHEETAH_DATA_DEFAULTALLOCATOR_H

#include "cheetah/Configuration.h"
#include "cheetah/utils/Architectures.h"

#ifdef SKA_CHEETAH_ENABLE_CUDA
#include "cheetah/cuda_utils/cuda_thrust.h"
#endif
#include "panda/DeviceAllocator.h"

#include <memory>

namespace ska {
namespace cheetah {
namespace data {


/**
 * @brief      Helper class for determining default allocators
 *
 * @details    The default allocator is determined by both compute architecture and
 *             value type. This class is predominantly used by the Series class to
 *             provide a more user friendly interface to both TimeSeries and
 *             FrequencySeries.
 *
 * @tparam     Arch  The type of compute architecture (e.g. cheeta::Cuda, cheetah::Cpu, etc.)
 * @tparam     T     The value type of the allocator
 */
template <typename Arch, typename T>
class DefaultAllocator
{
    public:
       typedef panda::DeviceAllocator<T, Arch> type;
};

#ifdef SKA_CHEETAH_ENABLE_CUDA
/**
 * DefaultAllocator specialisation for Cuda devices
 *
 * Uses thrust::device_malloc_allocator<T>.
 */
template <typename T>
class DefaultAllocator<cheetah::Cuda,T>
{
    public:
        typedef thrust::device_malloc_allocator<T> type;
};
#endif

/**
 * DefaultAllocator specialisation for general Cpus
 *
 * Uses std::allocator<T>.
 */
template <typename T>
class DefaultAllocator<cheetah::Cpu,T>
{
    public:
        typedef std::allocator<T> type;
};

template<typename ArchT, typename T>
using DefaultAllocator_t = typename DefaultAllocator<ArchT, T>::type;

} // namespace data
} // namespace cheetah
} // namespace ska

#endif //SKA_CHEETAH_DATA_DEFAULTALLOCATOR_H
