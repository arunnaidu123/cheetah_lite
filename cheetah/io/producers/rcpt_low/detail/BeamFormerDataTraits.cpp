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
#include "cheetah/io/producers/rcpt_low/BeamFormerDataTraits.h"
#include <panda/Log.h>
#include <algorithm>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {

template<class PacketTraits>
BeamFormerDataTraits<PacketTraits>::BeamFormerDataTraits()
{
}

template<class PacketTraits>
BeamFormerDataTraits<PacketTraits>::~BeamFormerDataTraits()
{

}

template<class PacketTraits>
uint64_t BeamFormerDataTraits<PacketTraits>::sequence_number(PacketInspector const& packet)
{
    return packet.sequence_number();
}

template<class PacketTraits>
std::size_t BeamFormerDataTraits<PacketTraits>::chunk_size(DataType const& chunk)
{
    return chunk.number_of_spectra() * chunk.number_of_channels();
}

template<class PacketTraits>
std::size_t BeamFormerDataTraits<PacketTraits>::packet_size()
{
    return (PacketInspector::Packet::number_of_samples()*PacketInspector::Packet::number_of_channels());
}

template<class PacketTraits>
std::size_t BeamFormerDataTraits<PacketTraits>::data_size()
{
    return PacketInspector::Packet::data_size();
}

template<class PacketTraits>
std::size_t BeamFormerDataTraits<PacketTraits>::packet_alignment_offset(PacketInspector const& inspector)
{
    return (inspector.packet().first_channel_number()/(PssLowTraits::number_of_channels/inspector.packet().number_of_channels()));
}

template<class PacketTraits>
void BeamFormerDataTraits<PacketTraits>::packet_stats(uint64_t packets_missing, uint64_t packets_expected)
{
    if(packets_missing > 0)
    {
        PANDA_LOG_WARN << "missing packets: " << packets_missing << " in " << packets_expected;
    }
}

template<class PacketTraits>
template<typename ContextType>
void BeamFormerDataTraits<PacketTraits>::deserialise_packet(ContextType& context, PacketInspector const& packet)
{
    typedef typename DataType::value_type ValueType;
    auto packet_it=packet.packet().begin();
    auto weights_it = packet.packet().begin_weights();
    auto it=context.chunk().begin();

    uint32_t number_of_samples = packet.packet().number_of_samples();
    uint32_t number_of_channels = packet.packet().number_of_channels();

    std::vector<int8_t> temp0(number_of_channels*number_of_samples*2);
    std::vector<int8_t> temp1(number_of_channels*number_of_samples*2);
    std::vector<ValueType> temp(number_of_channels*number_of_samples);

    panda::copy(packet_it, packet_it+number_of_channels*number_of_samples*2, temp0.begin());
    panda::copy(packet_it+number_of_channels*number_of_samples*2, packet_it+number_of_channels*number_of_samples*4, temp1.begin());

    for(uint32_t element=context.packet_offset(); element<context.packet_offset()+context.size(); element++)
    {
        uint32_t channel = element/number_of_samples;
        temp0[element*2] *= (*(weights_it+channel));
        temp0[element*2+1] *= (*(weights_it+channel));
        temp1[element*2] *= (*(weights_it+channel));
        temp1[element*2+1] *= (*(weights_it+channel));
        // divide by 4096 to scale back to 0-256
        temp[element] = (ValueType)(((int)temp0[element*2]*(int)temp0[element*2]) + ((int)temp0[element*2+1]*(int)temp0[element*2+1]) + ((int)temp1[element*2]*(int)temp1[element*2]) + ((int)temp1[element*2+1]*(int)temp1[element*2+1]));
    }
    panda::copy(temp.begin()+context.packet_offset(), temp.begin()+context.packet_offset()+context.size(), it+context.offset());
}

template<class PacketTraits>
template<typename ContextType>
void BeamFormerDataTraits<PacketTraits>::process_missing_slice(ContextType& context)
{
    // essentially replacing the data corresponding to the missing packet to 0. (Dont know what is the best approach here.)
    unsigned offset = context.offset();
    auto it=context.chunk().begin() + offset;
    PANDA_LOG_DEBUG << "processing missing packet: data=" << (void*)&*it << context;
    for(std::size_t i=0; i < context.size() ; ++i) {
        *it = 0;
        assert(it!=context.chunk().end());
        ++it;
    }
}

template<class PacketTraits>
template<typename ResizeContextType>
void BeamFormerDataTraits<PacketTraits>::resize_chunk(ResizeContextType& context)
{
    PANDA_LOG_DEBUG << "resizing data: " << context;
    // drops any incomplete spectra
    context.chunk().resize(data::DimensionSize<data::Time>(context.size()/context.chunk().number_of_channels()));
}

template<class PacketTraits>
constexpr typename BeamFormerDataTraits<PacketTraits>::PacketNumberType
BeamFormerDataTraits<PacketTraits>::max_sequence_number()
{
    return PacketInspector::Packet::max_sequence_number();
}

} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
