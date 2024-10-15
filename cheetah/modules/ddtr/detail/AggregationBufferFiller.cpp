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

#include "panda/AggregationBufferFiller.h"
#include "panda/Error.h"
#include <iterator>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename NumericalRep>
template<typename... Args>
AggregationBufferFiller<NumericalRep>::AggregationBufferFiller(FullBufferHandlerT listener, Args&& ... args)
    : _fn(listener)
    , _overlap(0)
    , _current(std::make_shared<AggregationBuffer<NumericalRep>>(std::forward<Args>(args)...))
{
    clock_gettime(CLOCK_MONOTONIC, &_t0);
}

template<typename NumericalRep>
AggregationBufferFiller<NumericalRep>::~AggregationBufferFiller()
{
}


template<typename NumericalRep>
bool AggregationBufferFiller<NumericalRep>::flush()
{
    // prepare a new buffer
    //unsigned long dedisp_samples_ns = (_current->number_of_spectra()-_overlap)*_current->sample_interval().value()*1e9;
    auto tmp(std::make_shared<AggregationBufferType>(_current->number_of_spectra(), _current->number_of_channels()));
    _current->swap(*tmp);

    if( _overlap != 0 )
        tmp->transfer(_overlap, *_current);

    // call our full buffer functor
    if(tmp->data_size()) {
        //clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &_t0, NULL);
        _fn(tmp);
        //add_nsec(_t0, dedisp_samples_ns);
        return true;
    }
    return false;
}

template<typename NumericalRep>
void AggregationBufferFiller<NumericalRep>::full_buffer_handler(FullBufferHandlerT const& handler)
{
    _fn = handler;
}

template<typename NumericalRep>
template<typename InsertDataType>
void AggregationBufferFiller<NumericalRep>::insert( InsertDataType const& data )
{
    _current->insert(data);
    if(_current->remaining_capacity() == 0)
    {
        flush();
    }
}

template<typename NumericalRep>
void AggregationBufferFiller<NumericalRep>::set_overlap(std::size_t const& overlap)
{
    _overlap = overlap;

    if( _overlap > this->size() ) {
        throw panda::Error("set_overlap() buffer too small for overlap");
    }
}

template<typename NumericalRep>
std::size_t AggregationBufferFiller<NumericalRep>::overlap() const
{
    return _overlap;
}

template<typename NumericalRep>
template<typename InsertDataType>
AggregationBufferFiller<NumericalRep>& AggregationBufferFiller<NumericalRep>::operator<<( InsertDataType const& data )
{
    insert(data);
    return *this;
}

template<typename NumericalRep>
std::size_t AggregationBufferFiller<NumericalRep>::size() const
{
    return _current->capacity();
}

template<typename NumericalRep>
std::size_t AggregationBufferFiller<NumericalRep>::remaining_capacity() const
{
    return _current->remaining_capacity();
}

template<typename NumericalRep>
void AggregationBufferFiller<NumericalRep>::resize(std::size_t number_of_spectra, std::size_t number_of_channels) const
{
    _current->resize(number_of_spectra, number_of_channels);
}

template<typename NumericalRep>
typename data::TimeFrequencyMetadata const& AggregationBufferFiller<NumericalRep>::metadata() const
{
    return _current->metadata();
}

template<typename NumericalRep>
void AggregationBufferFiller<NumericalRep>::metadata(typename data::TimeFrequencyMetadata const& metadata)
{
    _current->metadata(metadata);
}

//TODO: need to find a better way of doing this at present it is very ugly
template<typename NumericalRep>
void AggregationBufferFiller<NumericalRep>::add_nsec(struct timespec& temp, long nsec)
{
    temp.tv_nsec += nsec;
    if (temp.tv_nsec >= 1000000000)
    {
        long sec = temp.tv_nsec / 1000000000;
        temp.tv_sec += sec;
        temp.tv_nsec -= sec * 1000000000;
    }
    else if (temp.tv_nsec < 0)
    {
        long sec = temp.tv_nsec / 1000000000;
        sec -= 1;
        temp.tv_nsec -= sec * 1000000000;
        temp.tv_sec += sec;
    }
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
