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
#ifndef SKA_CHEETAH_UTILS_CONVOLVEPLAN_H
#define SKA_CHEETAH_UTILS_CONVOLVEPLAN_H

#include "cheetah/utils/Fftw3.h"
#include <cstddef>
#include <new>

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief Perform a convolution using fft methods
 */

class ConvolvePlan
{
    public:
        /**
         * @brief fast convolution of two data sets a and b of length npts
         *        output = a * b
         * @details note that a and b are overwritten
         */
        ConvolvePlan(std::size_t npts, float* a, float* b, float* output);
        ~ConvolvePlan();

        /**
         * @brief perform the convolution using the current data in a and b
         *        n.b. contents of a and b will be destroyed
         */
        void convolve();

    private:
        float* _a;
        float* _b;
        float* _output;
        std::size_t _npts;
        fftwf_complex* _c;
        fftwf_complex* _d;
        fftwf_plan _fwda;
        fftwf_plan _fwdb;
        fftwf_plan _bwd;
        float _scale;
};


} // namespace utils
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_UTILS_CONVOLVEPLAN_H
