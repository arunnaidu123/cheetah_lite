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
#include "cheetah/data/TimeFrequencyMetadata.h"

namespace ska {
namespace cheetah {
namespace data {


TimeFrequencyMetadata::TimeFrequencyMetadata()
{
}

TimeFrequencyMetadata::TimeFrequencyMetadata(TimeFrequencyMetadata const& metadata)
    : _frequency_index(metadata.channel_frequencies())
    , _sample_interval(metadata.sample_interval())
    , _start_time(metadata.start_time())
{
}

TimeFrequencyMetadata& TimeFrequencyMetadata::operator=(TimeFrequencyMetadata const& copy)
{
    _frequency_index = copy.channel_frequencies();
    _sample_interval = copy.sample_interval();
    _start_time = copy.start_time();
    return *this;
}

TimeFrequencyMetadata::TimeIntervalType const& TimeFrequencyMetadata::sample_interval() const
{
    return _sample_interval;
}

void TimeFrequencyMetadata::sample_interval(TimeFrequencyMetadata::TimeIntervalType dt)
{
    this->_sample_interval = dt;
}

std::vector<typename TimeFrequencyMetadata::FrequencyType> const& TimeFrequencyMetadata::channel_frequencies() const
{
    return this->_frequency_index;
}

std::pair<FrequencyType, FrequencyType> TimeFrequencyMetadata::low_high_frequencies() const
{
    if(_frequency_index.empty())
        return std::pair<FrequencyType, FrequencyType>(FrequencyType(0.0 * hz) , FrequencyType(0.0 * hz) );
    if(this->_frequency_index[0] > this->_frequency_index.back()) {
        return std::make_pair(_frequency_index.back(), _frequency_index.front());
    } else {
        return std::make_pair(_frequency_index[0], _frequency_index.back());
    }
}

void TimeFrequencyMetadata::channel_frequencies_const_width(TimeFrequencyMetadata::FrequencyType const& start
                                              , TimeFrequencyMetadata::FrequencyType const& delta
                                              , pss::astrotypes::DimensionSize<Frequency> number_of_channels)
{
    _frequency_index.resize(number_of_channels);
    for(unsigned i=0U; i < number_of_channels; ++i) {
        _frequency_index[i]= start + delta * (double)i;
    }
}

typename TimeFrequencyMetadata::TimePointType const& TimeFrequencyMetadata::start_time() const
{
    return this->_start_time;
}

typename TimeFrequencyMetadata::TimePointType TimeFrequencyMetadata::start_time(std::size_t offset) const
{
    return this->_start_time + std::chrono::duration<double>(sample_interval().value() * offset);
}

void TimeFrequencyMetadata::start_time(TimePointType const& start_time)
{
    this->_start_time = start_time;
}

} // namespace data
} // namespace cheetah
} // namespace ska
