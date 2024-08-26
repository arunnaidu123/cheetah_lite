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
#include "cheetah/modules/ddtr/detail/TimeFrequencyBufferFactory.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename TimeFrequencyType>
TimeFrequencyBufferFactory<TimeFrequencyType>::TimeFrequencyBufferFactory()
    : TimeFrequencyBufferFactory(data::DimensionSize<data::Time>(0)
                                ,data::DimensionSize<data::Frequency>(0))
{
}

template<typename TimeFrequencyType>
TimeFrequencyBufferFactory<TimeFrequencyType>::TimeFrequencyBufferFactory(data::DimensionSize<data::Time> number_of_spectra
                                                                   , data::DimensionSize<data::Frequency> number_of_channels)
    : _number_of_spectra(number_of_spectra)
    , _number_of_channels(number_of_channels)
{
}

template<typename TimeFrequencyType>
TimeFrequencyBufferFactory<TimeFrequencyType>::TimeFrequencyBufferFactory( data::DimensionSize<data::Frequency> number_of_channels
                                                                   ,  data::DimensionSize<data::Time> number_of_spectra)
    : TimeFrequencyBufferFactory(number_of_spectra, number_of_channels)
{
}

template<typename TimeFrequencyType>
TimeFrequencyType TimeFrequencyBufferFactory<TimeFrequencyType>::create_buffer() const
{
    return TimeFrequencyType(_number_of_spectra, _number_of_channels);
}

template<typename TimeFrequencyType>
data::DimensionSize<data::Time> TimeFrequencyBufferFactory<TimeFrequencyType>::size() const
{
    return _number_of_spectra;
}

template<typename TimeFrequencyType>
void TimeFrequencyBufferFactory<TimeFrequencyType>::resize(data::DimensionSize<data::Time> number_of_spectra)
{
    _number_of_spectra = number_of_spectra;
}

template<typename TimeFrequencyType>
void TimeFrequencyBufferFactory<TimeFrequencyType>::resize(data::DimensionSize<data::Time> number_of_spectra, data::DimensionSize<data::Frequency> number_of_channels)
{
    _number_of_spectra = number_of_spectra;
    _number_of_channels = number_of_channels;
}

template<typename TimeFrequencyType>
std::size_t TimeFrequencyBufferFactory<TimeFrequencyType>::number_of_elements(data::DimensionSize<data::Time> size) const
{
    return size * _number_of_channels;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
