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
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/corner_turn/CornerTurn.h"
#include "panda/Copy.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace data {

template <class Arch, typename NumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency()
    : BaseT( pss::astrotypes::DimensionSize<Frequency>(0)
           , pss::astrotypes::DimensionSize<Time>(0))
{}

template <class Arch, typename NumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency(DimensionSize<Time> number_of_spectra, DimensionSize<Frequency> number_of_channels)
    : BaseT( number_of_channels, number_of_spectra)
{
}

template <class Arch, typename NumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency(DimensionSize<Frequency> number_of_channels, DimensionSize<Time> number_of_spectra)
    : BaseT( number_of_channels, number_of_spectra)
{
}

template<class Arch, typename NumericalT>
template<class OtherArch, typename OtherNumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency(const TimeFrequency<OtherArch, OtherNumericalT>& tf)
    : BaseT(DimensionSize<Frequency>(tf.number_of_channels()), DimensionSize<Time>(tf.number_of_spectra()))
    , CommonBaseT(tf)
{
    panda::copy(tf.begin(), tf.end(), this->begin());
}

template <class Arch, typename NumericalT>
template<typename OtherNumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency(const FrequencyTime<Cpu, OtherNumericalT>& ft)
    : BaseT(ft)
    , CommonBaseT(ft)
{
}

template <class Arch, typename NumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency(FrequencyTime<Arch, NumericalT> const& ft)
    : BaseT(DimensionSize<Frequency>(ft.number_of_channels()), DimensionSize<Time>(ft.number_of_spectra()))
    , CommonBaseT(ft)
{
    corner_turn::corner_turn(ft.begin(), this->begin(), ft.number_of_spectra(), ft.number_of_channels());
}

template <class Arch, typename NumericalT>
template<typename OtherArch, typename OtherNumericalT>
TimeFrequency<Arch, NumericalT>::TimeFrequency(const FrequencyTime<OtherArch, OtherNumericalT>& ft)
    : BaseT(DimensionSize<Frequency>(ft.number_of_channels()), DimensionSize<Time>(ft.number_of_spectra()))
    , CommonBaseT(ft)
{
    TimeFrequency<OtherArch, NumericalT> tf_copy(ft);
    panda::copy(tf_copy.begin(), tf_copy.end(), this->begin());
}

template <class Arch, typename NumericalT>
std::size_t TimeFrequency<Arch, NumericalT>::number_of_channels() const
{
    return this->template dimension<Frequency>();
}

template <class Arch, typename NumericalT>
std::size_t TimeFrequency<Arch, NumericalT>::number_of_spectra() const
{
    return this->template dimension<Time>();
}

template<class Arch, typename NumericalT>
void TimeFrequency<Arch, NumericalT>::set_channel(unsigned channel_number, DataType const& data)
{
    Channel channel = this->channel(channel_number);
    std::fill(channel.begin(), channel.end(), data);
}

template<class Arch, typename NumericalT>
bool TimeFrequency<Arch, NumericalT>::operator==(TimeFrequency const& o) const
{
    return static_cast<BaseT const&>(*this) == static_cast<BaseT const&>(o);
}

template<typename T>
struct ExtractTimeFrequencyDataType : public ExtractTimeFrequencyDataTypeHelper<T>
{
};

template<typename T>
struct ExtractTimeFrequencyDataType<std::shared_ptr<T>>
{
    typedef typename ExtractTimeFrequencyDataType<T>::type type;
    inline static type& exctract(T& t) { return ExtractTimeFrequencyDataType<T>::extract(*t); }
};

template<typename T>
struct ExtractTimeFrequencyDataType<std::unique_ptr<T>>
{
    typedef typename ExtractTimeFrequencyDataType<T>::type type;
    inline static type& exctract(T& t) { return ExtractTimeFrequencyDataType<T>::extract(*t); }
};

template <class Arch, typename NumericalT>
struct ExtractTimeFrequencyDataType<TimeFrequency<Arch, NumericalT>>
{
    typedef TimeFrequency<Arch, NumericalT> type;
    inline static type& exctract(type& t) { return t; }
};

} // namespace data
} // namespace cheetah
} // namespace ska

