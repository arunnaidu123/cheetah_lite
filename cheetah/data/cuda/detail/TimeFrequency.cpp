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

#include "cheetah/corner_turn/cuda/CornerTurn.h"
#include "panda/Copy.h"

namespace ska {
namespace cheetah {
namespace data {


template <typename NumericalT>
TimeFrequency<Cuda, NumericalT>::TimeFrequency(TimeFrequency<Cuda, NumericalT> const& tf)
    : CommonBase(tf)
    , _data(tf.number_of_channels()*tf.number_of_spectra())
    , _number_of_channels(tf.number_of_channels())
    , _number_of_spectra(tf.number_of_spectra())
{
    panda::copy(tf.begin(), tf.end(), _data.begin());
}

template <typename NumericalT>
template <typename OtherArch, typename OtherNumericalT>
TimeFrequency<Cuda, NumericalT>::TimeFrequency(TimeFrequency<OtherArch, OtherNumericalT> const& tf)
    : CommonBase(tf)
    , _data(tf.number_of_channels()*tf.number_of_spectra())
    , _number_of_channels(tf.number_of_channels())
    , _number_of_spectra(tf.number_of_spectra())
{
    panda::copy(tf.begin(), tf.end(), _data.begin());
}

template <typename NumericalT>
template <typename OtherArch>
TimeFrequency<Cuda, NumericalT>::TimeFrequency(TimeFrequency<OtherArch, NumericalT> const& tf)
    : CommonBase(tf)
    , _data(tf.number_of_channels()*tf.number_of_spectra())
    , _number_of_channels(tf.number_of_channels())
    , _number_of_spectra(tf.number_of_spectra())
{
    panda::copy(tf.begin(), tf.end(), _data.begin());
}

template <typename NumericalT>
template <typename OtherNumericalT>
TimeFrequency<Cuda, NumericalT>::TimeFrequency(FrequencyTime<Cuda, OtherNumericalT> const& ft)
    : CommonBase(ft)
    , _data(ft.number_of_channels()*ft.number_of_spectra())
    , _number_of_channels(ft.number_of_channels())
    , _number_of_spectra(ft.number_of_spectra())
{
    corner_turn::cuda::corner_turn(ft.begin(), _data.begin(), ft.number_of_spectra(), ft.number_of_channels());
}


template <typename NumericalT>
template <typename OtherNumericalT>
TimeFrequency<Cuda, NumericalT>::TimeFrequency(FrequencyTime<Cpu, OtherNumericalT> const& ft)
    : CommonBase(ft)
    , _data(ft.number_of_channels()*ft.number_of_spectra())
    , _number_of_channels(ft.number_of_channels())
    , _number_of_spectra(ft.number_of_spectra())
{
    auto ft_copy = FrequencyTime<Cuda, OtherNumericalT>(ft);
    corner_turn::cuda::corner_turn(ft_copy.begin(), _data.begin(), ft.number_of_spectra(), ft.number_of_channels());
}

template <typename NumericalT>
typename TimeFrequency<Cuda, NumericalT>::Iterator TimeFrequency<Cuda, NumericalT>::begin()
{
    return _data.begin();
}

template <typename NumericalT>
typename TimeFrequency<Cuda, NumericalT>::ConstIterator TimeFrequency<Cuda, NumericalT>::begin() const
{
    return _data.begin();
}

template <typename NumericalT>
typename TimeFrequency<Cuda, NumericalT>::ConstIterator TimeFrequency<Cuda, NumericalT>::cbegin() const
{
    return _data.cbegin();
}

template <typename NumericalT>
typename TimeFrequency<Cuda, NumericalT>::Iterator TimeFrequency<Cuda, NumericalT>::end()
{
    return _data.end();
}

template <typename NumericalT>
typename TimeFrequency<Cuda, NumericalT>::ConstIterator TimeFrequency<Cuda, NumericalT>::end() const
{
    return _data.end();
}

template <typename NumericalT>
typename TimeFrequency<Cuda, NumericalT>::ConstIterator TimeFrequency<Cuda, NumericalT>::cend() const
{
    return _data.cend();
}

template <typename NumericalT>
std::size_t TimeFrequency<Cuda, NumericalT>::number_of_channels() const
{
    return _number_of_channels;
}

template <typename NumericalT>
std::size_t TimeFrequency<Cuda, NumericalT>::number_of_spectra() const
{
    return _number_of_spectra;
}

} //namespace ska
} //namespace cheetah
} //namespace data
