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
#ifndef SKA_CHEETAH_CUDA_UTILS_CUDA_THRUST_H
#define SKA_CHEETAH_CUDA_UTILS_CUDA_THRUST_H

#include "cheetah/version.h"
#include "cheetah/Configuration.h"

#ifdef SKA_CHEETAH_ENABLE_CUDA
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <cuda_runtime.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/device_ptr.h>
#include <thrust/device_malloc_allocator.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/transform_iterator.h>
#include <thrust/iterator/constant_iterator.h>
#include <thrust/iterator/zip_iterator.h>
#include <thrust/gather.h>
#include <thrust/functional.h>
#include <thrust/execution_policy.h>
#include <thrust/transform_reduce.h>
#include <thrust/for_each.h>
#include <thrust/complex.h>
#include <thrust/tuple.h>
#include <thrust/extrema.h>
#include <thrust/detail/raw_pointer_cast.h>
#include <thrust/reduce.h>
#include <thrust/sort.h>
#include <thrust/distance.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/execution_policy.h>
#include <thrust/system/cuda/execution_policy.h>
#include <thrust/system/cuda/vector.h>
#pragma GCC diagnostic pop

#if ((CHEETAH_CUDA_MAJOR_VERSION == 7) && (CHEETAH_CUDA_MINOR_VERSION == 5))
#define CUDA_BROKEN_THRUST_WITH_ALLOCATOR
#else // cuda 7.5
#if ((CHEETAH_CUDA_MAJOR_VERSION == 8) && (CHEETAH_CUDA_MINOR_VERSION == 0))
#define CUDA_BROKEN_THRUST_WITH_ALLOCATOR
#endif // cuda 8.0
#endif // broken thrust

#endif //SKA_CHEETAH_ENABLE_CUDA


namespace ska {
namespace cheetah {
namespace cuda_utils {

} // namespace cuda_utils
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_CUDA_UTILS_CUDA_THRUST_H
