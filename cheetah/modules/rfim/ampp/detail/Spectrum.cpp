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
#include "cheetah/modules/rfim/ampp/Spectrum.h"
#include <algorithm>
#include <numeric>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace ampp {


template<typename NumericalRep, typename Alloc>
Spectrum<NumericalRep, Alloc>::Spectrum(pss::astrotypes::DimensionSize<data::Frequency> size)
    : BaseT(size)
{
}

template<typename NumericalRep, typename Alloc>
Spectrum<NumericalRep, Alloc>::~Spectrum()
{
}

template<typename NumericalRep, typename Alloc>
void Spectrum<NumericalRep, Alloc>::adjust_mean(NumericalRep v)
{
    //std::transform(_dataBandPass.begin(), _dataBandPass.end(), _dataBandPass.begin(), bind2nd(std::plus<float>(), offset));
    float mean=0.0;
    auto delta = v - _mean;
    for(auto& val : *this) {
        val += delta;
        mean += val;
    }
    _mean = mean/this->template dimension<data::Frequency>();
}

template<typename NumericalRep, typename Alloc>
float Spectrum<NumericalRep, Alloc>::mean() const
{
    return _mean;
}

template<typename NumericalRep, typename Alloc>
float Spectrum<NumericalRep, Alloc>::calculate_mean()
{
    _mean = std::accumulate(this->begin(), this->end(), 0.0)/this->template dimension<data::Frequency>();
    return _mean;
}

} // namespace ampp
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
