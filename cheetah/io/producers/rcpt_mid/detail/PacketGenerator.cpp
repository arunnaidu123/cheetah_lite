/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#include "cheetah/io/producers/rcpt_mid/PacketGenerator.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {


template<typename DataGenerator>
PacketGenerator<DataGenerator>::PacketGenerator(DataGenerator& model
                                               ,PacketGeneratorConfig const& config
                                               )
    : _model(model)
    , _data(config.number_of_spectra(), config.number_of_channels())
    , _data_iterator(_data.cend())
    , _counter(0)
    , _interval(config.interval())
    , _max_buffers(5)
    , _buffer_index(0)
    , _number_of_samples_per_packet(config.number_of_spectra())
    , _number_of_channels_per_packet(config.number_of_channels_per_packet())
    , _bandwidth(config.bandwidth())
    , _fch1(config.fch1())
{
    _buffers.reserve(_max_buffers);
    for(unsigned i=0; i < _max_buffers; ++i) {
        _buffers.emplace_back(Packet::size());
    }
    _timestamp = ska::cheetah::utils::ModifiedJulianClock::now();
    _tsamp = ((double)config.number_of_channels()/static_cast<PacketGenerator<DataGenerator>::FrequencyType>(config.bandwidth()).value())*1e-6*data::seconds;
}

template<typename DataGenerator>
PacketGenerator<DataGenerator>::~PacketGenerator()
{
}

template<typename DataGenerator>
ska::panda::Buffer<char> PacketGenerator<DataGenerator>::next()
{
    typedef Packet::PacketType PacketType;
    ska::panda::Buffer<char>& buffer = _buffers[++_buffer_index%_max_buffers];

    char* ptr = buffer.data();
    auto packet = new(ptr) Packet; // placement new

    // fill the packet header.
    packet->packet_count(_counter/(PssMidTraits::number_of_channels/_number_of_channels_per_packet));
    packet->first_channel_number((_counter%(PssMidTraits::number_of_channels/_number_of_channels_per_packet))*_number_of_channels_per_packet);
    packet->packet_type(PacketType::PssMid); // Packet type
    packet->magic_word(25146554); // consitancy check (MAGICBEED)
    if(packet->first_channel_number()==0)
        _timestamp = _timestamp + (_number_of_samples_per_packet * _tsamp);
    static const std::chrono::time_point<std::chrono::system_clock> system_epoch;
    static const auto epoch_secs = std::chrono::time_point_cast<std::chrono::seconds>(ska::cheetah::utils::ModifiedJulianClock::time_point(system_epoch));
    auto secs = std::chrono::time_point_cast<std::chrono::seconds>(_timestamp);
    auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(_timestamp) - std::chrono::time_point_cast<std::chrono::nanoseconds>(secs);
    struct timespec ts{secs.time_since_epoch().count()-epoch_secs.time_since_epoch().count(), ns.count()};
    unsigned seconds = (unsigned)(ts.tv_sec);
    uint64_t attoseconds = (uint64_t)(ts.tv_nsec*1e9);

    packet->timestamp_seconds(seconds);
    packet->timestamp_attoseconds(attoseconds);
    packet->set_unit_weights();
    packet->channel_separation((uint32_t)((static_cast<PacketGenerator<DataGenerator>::FrequencyType>(_bandwidth).value()/(double)PssMidTraits::number_of_channels)*1e9));
    packet->first_channel_frequency(_fch1.value());
    auto packet_it = packet->begin();
    if(_data_iterator == _data.cend())
    {
        // get next chunk of data from the model
        _model.next(_data);
        _data_iterator = _data.cbegin();
    }

    // Fill the packet data.
    panda::copy(_data_iterator, _data_iterator+(Packet::number_of_channels() * Packet::number_of_samples()), packet_it);
    _data_iterator += Packet::number_of_channels() * Packet::number_of_samples();
    ++_counter;
    return buffer;
}

template<typename DataGenerator>
std::chrono::microseconds PacketGenerator<DataGenerator>::interval() const
{
    return _interval;
}

template<typename DataGenerator>
void PacketGenerator<DataGenerator>::abort()
{
}

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
