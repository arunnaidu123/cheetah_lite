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
#include "cheetah/io/producers/rcpt/PacketGenerator.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {


template<typename DataGenerator>
PacketGenerator<DataGenerator>::PacketGenerator(DataGenerator& model
                                               ,data::DimensionSize<data::Frequency> number_of_channels
                                               ,data::DimensionSize<data::Time> number_of_time_samples
                                               )
    : _model(model)
    , _data(number_of_time_samples, number_of_channels)
    , _data_iterator(_data.cend())
    , _counter(0)
    , _interval(1)
    , _max_buffers(5)
    , _buffer_index(0)
{
    _buffers.reserve(_max_buffers);
    for(unsigned i=0; i < _max_buffers; ++i) {
        _buffers.emplace_back(Packet::size());
    }
}

template<typename DataGenerator>
PacketGenerator<DataGenerator>::~PacketGenerator()
{
}

template<typename DataGenerator>
ska::panda::Buffer<char> PacketGenerator<DataGenerator>::next()
{
    ska::panda::Buffer<char>& buffer = _buffers[++_buffer_index%_max_buffers];
    //ska::panda::Buffer<char>& buffer(BeamFormerPacket::size());

    char* ptr = buffer.data();
    auto packet = new(ptr) Packet; // placement new

    // fill the packet header.
    packet->packet_count(_counter);
    packet->first_channel_number(0);
    packet->packet_type(PacketType::StokesI);

    for (unsigned i = 0; i < Packet::number_of_samples(); ++i) {
        if(_data_iterator == _data.cend())
        {
            // get next chunk of data from the model
            _model.next(_data);
            _data_iterator = _data.cbegin();
        }

        // Fill the packet data.
        packet->insert(i, Sample(
                       *_data_iterator,
                       *_data_iterator,
                       0,
                       0));
        ++_data_iterator;
    }

    // Increment counters for next time.
    ++_counter;
    return buffer;
}

template<typename DataGenerator>
long PacketGenerator<DataGenerator>::interval() const
{
    return _interval;
}

template<typename DataGenerator>
void PacketGenerator<DataGenerator>::abort()
{
}

} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
