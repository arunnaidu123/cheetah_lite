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
#include "cheetah/data/PowerSeries.h"


namespace ska {
namespace cheetah {
namespace data {

using boost::math::complement;
using boost::math::cdf;
using boost::math::quantile;

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
PowerSeries<Arch, ValueType, num_spectra, Alloc>::PowerSeries(std::size_t size, Allocator const& allocator)
    : PowerSeries<Arch, ValueType, num_spectra, Alloc>(1.0 * hz, 2, size*num_spectra, allocator)
{
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
PowerSeries<Arch, ValueType, num_spectra, Alloc>::~PowerSeries()
{
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
PowerSeries<Arch, ValueType, num_spectra, Alloc>::PowerSeries(FourierFrequencyType const& df, double degrees_of_freedom
                                                , std::size_t size, Allocator const& allocator)
    : FrequencySeries<Arch,ValueType,Alloc>(size, allocator)
    , _dof(degrees_of_freedom)
    , _distribution(_dof)
    , _gaussian()
{
    this->frequency_step(df);
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
double PowerSeries<Arch, ValueType, num_spectra, Alloc>::degrees_of_freedom() const
{
    return _dof;
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
void PowerSeries<Arch, ValueType, num_spectra, Alloc>::degrees_of_freedom(double degrees_of_freedom)
{
    _dof = degrees_of_freedom;
    _distribution = ChiSquaredType(_dof);
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
double PowerSeries<Arch, ValueType, num_spectra, Alloc>::pvalue(float power) const
{
    return cdf(complement(_distribution,(double) power));
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
float PowerSeries<Arch, ValueType, num_spectra, Alloc>::power_to_equiv_sigma(float power) const
{
    return (float) quantile(complement(_gaussian,pvalue(power)));
}

template <typename Arch, typename ValueType, unsigned num_spectra, typename Alloc>
float PowerSeries<Arch, ValueType, num_spectra, Alloc>::equiv_sigma_to_power(float sigma) const
{
    double p = cdf(complement(_gaussian,(double) sigma));
    return (float) quantile(complement(_distribution,p));
}

} // namespace data
} // namespace cheetah
} // namespace ska
