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
#include "cheetah/utils/ConvolvePlan.h"


namespace ska {
namespace cheetah {
namespace utils {


ConvolvePlan::ConvolvePlan(std::size_t npts, float* a, float* b, float* output)
    : _a(a)
    , _b(b)
    , _output(output)
    , _npts(npts)
    , _c(fftwf_alloc_complex(npts))
{
    if(_c == nullptr) throw std::bad_alloc();
    _d=fftwf_alloc_complex(npts);
    if(_d == nullptr) { 
        fftwf_free(_c);
        throw std::bad_alloc();
    }
    _fwda = fftwf_plan_dft_r2c_1d(npts, _a, _c, FFTW_MEASURE);
    if(_fwda == nullptr) {
        fftwf_free(_c);
        fftwf_free(_d);
        throw std::bad_alloc();
    }
    _fwdb = fftwf_plan_dft_r2c_1d(npts, _b, _d, FFTW_MEASURE);
    if(_fwdb == nullptr) {
        fftwf_free(_c);
        fftwf_free(_d);
        fftwf_destroy_plan(_fwda);
        throw std::bad_alloc();
    }
    _bwd  = fftwf_plan_dft_c2r_1d(npts, _c, _output, FFTW_MEASURE);
    if(_bwd == nullptr) {
        fftwf_free(_c);
        fftwf_free(_d);
        fftwf_destroy_plan(_fwda);
        fftwf_destroy_plan(_fwdb);
        throw std::bad_alloc();
    }
    _scale = (float) npts;
}

ConvolvePlan::~ConvolvePlan()
{
    fftwf_free(_c);
    fftwf_free(_d);
    fftwf_destroy_plan(_fwda);
    fftwf_destroy_plan(_fwdb);
    fftwf_destroy_plan(_bwd);
}

void ConvolvePlan::convolve()
{
    std::size_t i, j;
    fftwf_execute(_fwda);
    fftwf_execute(_fwdb);
    for(i = 0; i < _npts; ++i) {
        for(j = 0; j < 2; ++j) {
            _c[i][j] *= _d[i][j]/_scale;
        }
    }
    fftwf_execute(_bwd);
}

} // namespace utils
} // namespace cheetah
} // namespace ska
