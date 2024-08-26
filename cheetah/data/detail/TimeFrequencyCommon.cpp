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
#include <iostream>
namespace ska {
namespace cheetah {
namespace data {

template <class DerivedType>
TimeFrequencyCommon<DerivedType>::TimeFrequencyCommon()
{
}

template <class DerivedType>
template <typename OtherDerivedType>
TimeFrequencyCommon<DerivedType>::TimeFrequencyCommon(TimeFrequencyCommon<OtherDerivedType> const& tfc)
    : _metadata(tfc.metadata())
{
}

template <class DerivedType>
inline
typename TimeFrequencyCommon<DerivedType>::TimePointType const& TimeFrequencyCommon<DerivedType>::start_time() const
{
    return this->_metadata.start_time();
}

template <class DerivedType>
inline
typename TimeFrequencyCommon<DerivedType>::TimePointType TimeFrequencyCommon<DerivedType>::start_time(std::size_t offset) const
{
    return this->_metadata.start_time(offset);
}

template <class DerivedType>
void TimeFrequencyCommon<DerivedType>::start_time(TimePointType const& start_time)
{
    this->_metadata.start_time(start_time);
}

template <class DerivedType>
typename TimeFrequencyCommon<DerivedType>::TimePointType TimeFrequencyCommon<DerivedType>::end_time() const
{
    auto derived = static_cast<DerivedType const&>(*this);
    return this->start_time() + (derived.number_of_spectra() - 1) * sample_interval();
}

template <class DerivedType>
inline
typename TimeFrequencyCommon<DerivedType>::TimeType TimeFrequencyCommon<DerivedType>::sample_interval() const
{
    return this->_metadata.sample_interval();
}

template <class DerivedType>
inline
void TimeFrequencyCommon<DerivedType>::sample_interval(TimeFrequencyCommon<DerivedType>::TimeType dt)
{
    this->_metadata.sample_interval(dt);
}

template <class DerivedType>
std::vector<typename TimeFrequencyCommon<DerivedType>::FrequencyType> const& TimeFrequencyCommon<DerivedType>::channel_frequencies() const
{
    return this->_metadata.channel_frequencies();
}

template <class DerivedType>
std::pair<FrequencyType, FrequencyType> TimeFrequencyCommon<DerivedType>::low_high_frequencies() const
{
    return this->_metadata.low_high_frequencies();
}

template <class DerivedType>
void TimeFrequencyCommon<DerivedType>::set_channel_frequencies_const_width(TimeFrequencyCommon<DerivedType>::FrequencyType const& start,
                                              TimeFrequencyCommon<DerivedType>::FrequencyType const& delta)
{
    auto derived = static_cast<DerivedType const&>(*this);
    this->_metadata.channel_frequencies_const_width(start, delta, data::DimensionSize<Frequency>(derived.number_of_channels()));
}

template <class DerivedType>
template<class InputIterator>
void TimeFrequencyCommon<DerivedType>::set_channel_frequencies(InputIterator begin, InputIterator const end)
{
    this->_metadata.channel_frequencies(begin, end);
}

template <class DerivedType>
TimeFrequencyMetadata const& TimeFrequencyCommon<DerivedType>::metadata() const
{
    return _metadata;
}

template <class DerivedType>
void TimeFrequencyCommon<DerivedType>::metadata(TimeFrequencyMetadata const& metadata)
{
    _metadata = metadata;
}

} // namespace data
} // namespace cheetah
} // namespace ska
