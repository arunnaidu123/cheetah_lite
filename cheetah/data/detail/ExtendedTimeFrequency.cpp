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
#include "cheetah/data/ExtendedTimeFrequency.h"
#include "cheetah/data/TimeFrequency.h"

namespace ska {
namespace cheetah {
namespace data {

template<typename TimeFrequencyT>
ExtendedTimeFrequency<TimeFrequencyT>::ExtendedTimeFrequency(std::shared_ptr<TimeFrequencyT> const& data)
    : BaseT(data)
{
}

template<typename TimeFrequencyT>
ExtendedTimeFrequency<TimeFrequencyT>::~ExtendedTimeFrequency()
{
}

template<typename TimeFrequencyT>
ExtendedTimeFrequency<TimeFrequencyT>::operator typename ExtendedTimeFrequency<TimeFrequencyT>::Type const&() const
{
    return this->get();
}

template<typename TimeFrequencyT>
inline
typename ExtendedTimeFrequency<TimeFrequencyT>::TimePointType const& ExtendedTimeFrequency<TimeFrequencyT>::start_time() const
{
    return this->get().start_time();
}

template<typename TimeFrequencyT>
inline
typename ExtendedTimeFrequency<TimeFrequencyT>::TimePointType ExtendedTimeFrequency<TimeFrequencyT>::start_time(std::size_t offset) const
{
    return this->get().start_time(offset);
}

template<typename TimeFrequencyT>
inline
void ExtendedTimeFrequency<TimeFrequencyT>::start_time(typename ExtendedTimeFrequency<TimeFrequencyT>::TimePointType const& time_point)
{
    this->get().start_time(time_point);
}

template<typename TimeFrequencyT>
inline
typename ExtendedTimeFrequency<TimeFrequencyT>::TimeType ExtendedTimeFrequency<TimeFrequencyT>::sample_interval() const
{
    return this->get().sample_interval();
}

template<typename TimeFrequencyT>
inline
void ExtendedTimeFrequency<TimeFrequencyT>::sample_interval(typename ExtendedTimeFrequency<TimeFrequencyT>::TimeType interval)
{
    return this->get().sample_interval(interval);
}

template<typename TimeFrequencyT>
inline
std::vector<typename ExtendedTimeFrequency<TimeFrequencyT>::FrequencyType> const& ExtendedTimeFrequency<TimeFrequencyT>::channel_frequencies() const
{
    return this->get().channel_frequencies();
}

template<typename TimeFrequencyT>
inline
std::pair<typename ExtendedTimeFrequency<TimeFrequencyT>::FrequencyType, typename ExtendedTimeFrequency<TimeFrequencyT>::FrequencyType> ExtendedTimeFrequency<TimeFrequencyT>::low_high_frequencies() const
{
    return this->get().low_high_frequencies();
}

template<typename TimeFrequencyT>
inline
void ExtendedTimeFrequency<TimeFrequencyT>::set_channel_frequencies_const_width(FrequencyType const& start, FrequencyType const& delta)
{
    this->get().set_channel_frequencies_const_width(start, delta);
}

template<typename TimeFrequencyT>
inline
void ExtendedTimeFrequency<TimeFrequencyT>::set_channel(unsigned channel_number, NumericalRep const& value)
{
    this->get().set_channel(channel_number, value);
}

// specialisation for Extended data types
template<typename T>
struct ExtractTimeFrequencyDataTypeHelper<T, typename std::enable_if<std::is_base_of<ExtendedTimeFrequencyTag, T>::value>::type>
{
    typedef typename T::TimeFrequencyType type;
    inline static type& extract(T& t) { return static_cast<type&>(t); }
    inline static type const& extract(T const& t) { return static_cast<type const&>(t); }
};


} // namespace data
} // namespace cheetah
} // namespace ska
