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

#include "cheetah/Configuration.h"

#ifdef SKA_CHEETAH_ENABLE_CUDA
#ifdef __CUDACC__

#include <cuda.h>
#include <cuda_runtime.h>

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace cuda {

template <typename InputNumericalT, typename OutputNumericalT>
__global__ void simple_corner_turn_kernel(const InputNumericalT *d_input, OutputNumericalT *d_output, std::size_t first_dimension, std::size_t second_dimension)
{
    int t = blockIdx.x * blockDim.x + threadIdx.x;
    if(t < first_dimension*second_dimension)
    {
        int c = t%first_dimension;
        int s = t/first_dimension;
        d_output[c * second_dimension + s] = (OutputNumericalT)(d_input[s * first_dimension + c]);
    }
}

} //namespace cuda
} //namespace corner_turn
} //namespace cheetah
} //namespace ska

#endif // __CUDACC__
#endif // SKA_CHEETAH_ENABLE_CUDA
