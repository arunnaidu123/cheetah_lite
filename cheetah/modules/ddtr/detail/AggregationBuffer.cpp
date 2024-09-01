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

//#include "panda/AggregationBuffer.h"
#include "panda/Log.h"
#include "panda/Copy.h"
#include <cstring>
#include <algorithm>
#include <utility>
#include <iterator>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename NumericalRep>
AggregationBuffer<NumericalRep>::AggregationBuffer()
    : _buffer(std::make_unique<std::vector<NumericalRep>>(0))
    , _time_stride(1)
    , _frequency_stride(0)
    , _number_of_spectra(0)
    , _number_of_channels(0)
    , _current_time(0)
{
}

template<typename NumericalRep>
AggregationBuffer<NumericalRep>::AggregationBuffer(std::size_t number_of_spectra, std::size_t number_of_channels)
    : _buffer(std::make_unique<std::vector<NumericalRep>>(number_of_channels*number_of_spectra))
    , _time_stride(1)
    , _frequency_stride(number_of_spectra)
    , _number_of_spectra(number_of_spectra)
    , _number_of_channels(number_of_channels)
    , _current_time(0)
{
}

template<typename NumericalRep>
AggregationBuffer<NumericalRep>::AggregationBuffer(AggregationBuffer<NumericalRep>&& b)
    : _buffer(std::move(b._buffer))
    , _time_stride(std::move(b._time_stride))
    , _frequency_stride(std::move(b._frequency_stride))
    , _number_of_spectra(std::move(b._number_of_spectra))
    , _number_of_channels(std::move(b._number_of_channels))
    , _current_time(std::move(b._current_time))
{
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::resize(std::size_t number_of_spectra, std::size_t number_of_channels)
{
    _buffer->resize(number_of_channels*number_of_spectra);
    _time_stride= 1;
    _frequency_stride = number_of_spectra;
    _number_of_spectra = number_of_spectra;
    _number_of_channels = number_of_channels;
    _current_time = 0;
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::swap(AggregationBuffer& b)
{
    std::swap(_buffer, b._buffer);
    std::swap(b._time_stride, _time_stride);
    std::swap(b._frequency_stride, _frequency_stride);
    std::swap(b._number_of_spectra, _number_of_spectra);
    std::swap(b._number_of_channels, _number_of_channels);
    std::swap(b._current_time, _current_time);
    b.metadata(_metadata);
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::BufferType& AggregationBuffer<NumericalRep>::buffer()
{
    return _buffer;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::BufferType const& AggregationBuffer<NumericalRep>::buffer() const
{
    return _buffer;
}

template<typename NumericalRep>
AggregationBuffer<NumericalRep>::~AggregationBuffer()
{
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::iterator AggregationBuffer<NumericalRep>::data(unsigned offset)
{
    return _buffer->begin() + offset;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::data(unsigned offset) const
{
    return _buffer->cbegin() + offset;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::iterator AggregationBuffer<NumericalRep>::begin()
{
    return _buffer->begin();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::begin() const
{
    return _buffer->cbegin();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::cbegin() const
{
    return _buffer->cbegin();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::iterator AggregationBuffer<NumericalRep>::begin(unsigned frequency_channel)
{
    return _buffer->begin() + frequency_channel*_frequency_stride;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::begin(unsigned frequency_channel) const
{
    return _buffer->cbegin() + frequency_channel*_frequency_stride;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::cbegin(unsigned frequency_channel) const
{
    return _buffer->cbegin() + frequency_channel*_frequency_stride;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::iterator AggregationBuffer<NumericalRep>::end()
{
    return  _buffer->end();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::end() const
{
    return _buffer->cend();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::cend() const
{
    return _buffer->cend();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::iterator AggregationBuffer<NumericalRep>::end(unsigned frequency_channel)
{
    return  _buffer->end() + frequency_channel*_frequency_stride;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::end(unsigned frequency_channel) const
{
    return _buffer->cend() + frequency_channel*_frequency_stride;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::const_iterator AggregationBuffer<NumericalRep>::cend(unsigned frequency_channel) const
{
    return _buffer->cend() + frequency_channel*_frequency_stride;
}

template<typename NumericalRep>
std::size_t AggregationBuffer<NumericalRep>::data_size() const
{
    return _current_time;
}

template<typename NumericalRep>
std::size_t AggregationBuffer<NumericalRep>::capacity() const
{
    return _number_of_spectra;
}

template<typename NumericalRep>
std::size_t AggregationBuffer<NumericalRep>::number_of_spectra() const
{
    return _number_of_spectra;
}

template<typename NumericalRep>
std::size_t AggregationBuffer<NumericalRep>::number_of_channels() const
{
    return _number_of_channels;
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::insert(FrequencyTimeType const& object )
{
    if(_current_time==0)
    {
        this->metadata(object.metadata());
    }

    if(remaining_capacity()>=object.number_of_spectra())
    {
        for(unsigned int channel=0; channel<object.number_of_channels(); ++channel)
        {
            panda::copy(object.begin()+channel*object.number_of_spectra()
                       , object.begin()+((channel+1)*object.number_of_spectra())
                       , this->begin(channel)+_current_time
                       );
        }
        _current_time += object.number_of_spectra();
    }
    else
    {
        for(unsigned int channel=0; channel<object.number_of_channels(); ++channel)
        {
            panda::copy( object.begin()+channel*object.number_of_spectra()
                       , object.begin()+(channel*object.number_of_spectra()+remaining_capacity())
                       , this->begin(channel)+_current_time
                       );
        }
        _current_time += remaining_capacity();
    }
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::insert(TimeFrequencyType const& object)
{
    FrequencyTimeType temp(object);
    insert(temp);
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::insert(std::shared_ptr<TimeFrequencyType> const& object)
{
    FrequencyTimeType temp(*object);
    insert(temp);
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::insert(std::shared_ptr<FrequencyTimeType> const& object)
{
    insert(*object);
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::transfer(std::size_t size, AggregationBuffer<NumericalRep>& dest) const
{
    if(size>this->capacity())
    {
        PANDA_LOG<<"buffer size is smaller than overlap";
    }
    dest.start_time(this->start_time(this->capacity()-size));

    for(unsigned int channel=0; channel<dest.number_of_channels(); ++channel)
    {
        panda::copy( this->begin(channel)+this->capacity()-size
                   , this->begin(channel)+this->capacity()
                   , dest.begin(channel)
                   );
    }
    dest._current_time += size;
}

template<typename NumericalRep>
std::size_t AggregationBuffer<NumericalRep>::remaining_capacity() const
{
    return _number_of_spectra-_current_time;
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::TimePointType const& AggregationBuffer<NumericalRep>::start_time() const
{
    return this->_metadata.start_time();
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::TimePointType AggregationBuffer<NumericalRep>::start_time(std::size_t offset) const
{
    return this->_metadata.start_time(offset);
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::start_time(TimePointType const& start_time)
{
    this->_metadata.start_time(start_time);
}

template<typename NumericalRep>
typename AggregationBuffer<NumericalRep>::TimeType AggregationBuffer<NumericalRep>::sample_interval() const
{
    return this->_metadata.sample_interval();
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::sample_interval(AggregationBuffer<NumericalRep>::TimeType dt)
{
    this->_metadata.sample_interval(dt);
}

template<typename NumericalRep>
std::vector<typename AggregationBuffer<NumericalRep>::FrequencyType> const& AggregationBuffer<NumericalRep>::channel_frequencies() const
{
    return this->_metadata.channel_frequencies();
}

template<typename NumericalRep>
std::pair<typename AggregationBuffer<NumericalRep>::FrequencyType, typename AggregationBuffer<NumericalRep>::FrequencyType> AggregationBuffer<NumericalRep>::low_high_frequencies() const
{
    return this->_metadata.low_high_frequencies();
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::set_channel_frequencies_const_width(AggregationBuffer<NumericalRep>::FrequencyType const& start,
                                              AggregationBuffer<NumericalRep>::FrequencyType const& delta)
{
    this->_metadata.channel_frequencies_const_width(start, delta, data::DimensionSize<data::Frequency>(number_of_channels()));
}

template<typename NumericalRep>
template<class InputIterator>
void AggregationBuffer<NumericalRep>::set_channel_frequencies(InputIterator begin, InputIterator const end)
{
    this->_metadata.channel_frequencies(begin, end);
}

template<typename NumericalRep>
typename data::TimeFrequencyMetadata const& AggregationBuffer<NumericalRep>::metadata() const
{
    return _metadata;
}

template<typename NumericalRep>
void AggregationBuffer<NumericalRep>::metadata(typename data::TimeFrequencyMetadata const& metadata)
{
    _metadata = metadata;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska