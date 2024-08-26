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
FrequencyTime<Cuda, NumericalT>::FrequencyTime(FrequencyTime<Cuda,NumericalT> const& ft)
    : CommonBase(ft)
    , _data(ft.number_of_channels()*ft.number_of_spectra())
    , _number_of_channels(ft.number_of_channels())
    , _number_of_spectra(ft.number_of_spectra())
{
    panda::copy(ft.begin(), ft.end(), _data.begin());
}

template <typename NumericalT>
template<typename OtherArch, typename OtherNumericalT>
FrequencyTime<Cuda, NumericalT>::FrequencyTime(FrequencyTime<OtherArch, OtherNumericalT> const& ft)
    : CommonBase(ft)
    , _data(ft.number_of_channels()*ft.number_of_spectra())
    , _number_of_channels(ft.number_of_channels())
    , _number_of_spectra(ft.number_of_spectra())
{
    panda::copy(ft.begin(), ft.end(), _data.begin());
}

template <typename NumericalT>
template <typename OtherNumericalT>
FrequencyTime<Cuda, NumericalT>::FrequencyTime(TimeFrequency<Cuda, OtherNumericalT> const& tf)
    : CommonBase(tf)
    , _data(tf.number_of_channels()*tf.number_of_spectra())
    , _number_of_channels(tf.number_of_channels())
    , _number_of_spectra(tf.number_of_spectra())
{
    corner_turn::cuda::corner_turn(tf.begin(), this->begin(), tf.number_of_channels(), tf.number_of_spectra());
}

template <typename NumericalT>
template <typename OtherNumericalT>
FrequencyTime<Cuda, NumericalT>::FrequencyTime(TimeFrequency<Cpu, OtherNumericalT> const& tf)
    : CommonBase(tf)
    , _data(tf.number_of_channels()*tf.number_of_spectra())
    , _number_of_channels(tf.number_of_channels())
    , _number_of_spectra(tf.number_of_spectra())
{
    TimeFrequency<Cuda, OtherNumericalT> tf_copy(tf);
    corner_turn::cuda::corner_turn(tf_copy.begin(), _data.begin(), tf.number_of_channels(), tf.number_of_spectra());
}

template <typename NumericalT>
typename FrequencyTime<Cuda, NumericalT>::Iterator FrequencyTime<Cuda, NumericalT>::begin()
{
    return _data.begin();
}

template <typename NumericalT>
typename FrequencyTime<Cuda, NumericalT>::ConstIterator FrequencyTime<Cuda, NumericalT>::begin() const
{
    return _data.begin();
}

template <typename NumericalT>
typename FrequencyTime<Cuda, NumericalT>::ConstIterator FrequencyTime<Cuda, NumericalT>::cbegin() const
{
    return _data.cbegin();
}

template <typename NumericalT>
typename FrequencyTime<Cuda, NumericalT>::Iterator FrequencyTime<Cuda, NumericalT>::end()
{
    return _data.end();
}

template <typename NumericalT>
typename FrequencyTime<Cuda, NumericalT>::ConstIterator FrequencyTime<Cuda, NumericalT>::end() const
{
    return _data.end();
}

template <typename NumericalT>
typename FrequencyTime<Cuda, NumericalT>::ConstIterator FrequencyTime<Cuda, NumericalT>::cend() const
{
    return _data.cend();
}

template <typename NumericalT>
std::size_t FrequencyTime<Cuda, NumericalT>::number_of_channels() const
{
    return _number_of_channels;
}

template <typename NumericalT>
std::size_t FrequencyTime<Cuda, NumericalT>::number_of_spectra() const
{
    return _number_of_spectra;
}

} //namespace ska
} //namespace cheetah
} //namespace data
