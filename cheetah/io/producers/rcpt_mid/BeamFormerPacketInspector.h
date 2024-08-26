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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_BEAMFORMERPACKETINSPECTOR_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_BEAMFORMERPACKETINSPECTOR_H

#include "ska/cbf_psr_interface/CbfPsrPacket.h"
#include "cheetah/io/producers/rcpt_mid/MidIcdParameters.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

/**
 * @brief
 *      BeamFormerPacket inspection and data extraction
 */
template<class PacketTraits>
class BeamFormerPacketInspector
{
    public:
        typedef typename ska::cbf_psr_interface::CbfPsrPacket<typename PacketTraits::PacketDataType, PacketTraits::number_of_samples_per_packet, PacketTraits::number_of_channels_per_packet> Packet;
        typedef typename Packet::PacketNumberType PacketNumberType;

    public:

        BeamFormerPacketInspector(Packet const& packet);
        ~BeamFormerPacketInspector();

        /**
         * @brief return the sequence number embedded in the packet
         */
        PacketNumberType sequence_number() const;

        /**
         * @brief return true if no further processing is required on this packet
         */
        bool ignore() const;

        /**
         * @brief return the packet
         */
        inline Packet const& packet() const;

    private:
        Packet const& _packet;
};


typedef BeamFormerPacketInspector<PssMidTraits> PacketInspectorMid;
typedef PacketInspectorMid::Packet BeamFormerPacketMid;

static_assert(sizeof(BeamFormerPacketMid)==PssMidTraits::size_of_packet,"BeamFormerPacketLow Packet size is not as expected");

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/producers/rcpt_mid/detail/BeamFormerPacketInspector.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_BEAMFORMERPACKETINSPECTOR_H
