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
#include "cheetah/io/producers/rcpt_mid/BeamFormerPacketInspector.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

template<class PacketTraits>
typename BeamFormerPacketInspector<PacketTraits>::Packet const&
BeamFormerPacketInspector<PacketTraits>::packet() const
{
    return _packet;
}

template<class PacketTraits>
BeamFormerPacketInspector<PacketTraits>::BeamFormerPacketInspector(Packet const & packet)
    : _packet(packet)
{
}

template<class PacketTraits>
BeamFormerPacketInspector<PacketTraits>::~BeamFormerPacketInspector()
{
}

template<class PacketTraits>
typename BeamFormerPacketInspector<PacketTraits>::PacketNumberType BeamFormerPacketInspector<PacketTraits>::sequence_number() const
{
    PacketNumberType value = ((PacketNumberType)_packet.packet_count())*(PssMidTraits::number_of_channels/((PacketNumberType)_packet.number_of_channels()))+(((PacketNumberType)(_packet.first_channel_number()))/((PacketNumberType)_packet.number_of_channels()));
    return value;
}

template<class PacketTraits>
bool BeamFormerPacketInspector<PacketTraits>::ignore() const
{
    return (_packet.magic_word()==25146554)?false:true;
}

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
