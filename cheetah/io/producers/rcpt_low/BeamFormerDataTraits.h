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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_BEAMFORMERDATATRAITS_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_BEAMFORMERDATATRAITS_H

#include "cheetah/io/producers/rcpt_low/BeamFormerPacketInspector.h"
#include "cheetah/data/TimeFrequency.h"


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {

/**
 * @brief
 *    Traits describing the BeamFormer Data Stream to the panda::PacketSream system
 *
 * @details
 *
 */
template<class PacketTraits>
class BeamFormerDataTraits
{

    public:
        typedef data::FrequencyTime<Cpu, uint16_t> DataType;
        typedef BeamFormerPacketInspector<PacketTraits> PacketInspector;
        typedef typename PacketInspector::PacketNumberType PacketNumberType;
        static constexpr std::size_t number_of_blocks = 800;
        static constexpr std::size_t contexts_per_block = 10;

    public:
        BeamFormerDataTraits();
        ~BeamFormerDataTraits();

        /**
         * @brief returns the sequential number of the chunk that the packet belongs to
         * @details do not call this function before a call to packets_per_chunk has been made at least once
         */
        static uint64_t sequence_number(PacketInspector const& packet);

        /**
         * @brief the maximum value a packet sequence number will reach
         */
        static constexpr PacketNumberType max_sequence_number();

        /**
         * @brief return the number of signal samples in a packet (Arun: Need to rename)
         */
        static std::size_t packet_size();

        /**
         * @brief size of the data in the packet excluding header and weights
         */
        static std::size_t data_size();

        /**
         * @brief return the total number of samples (time_samples * channels) in the data
         */
        static std::size_t chunk_size(DataType const& data);

        /**
         * @brief ignore all packets other than the StokesI
         */
        static std::size_t packet_alignment_offset(PacketInspector const& packet);

        /**
         * @brief return the total number of samples (time_samples * channels) in the data
         */
        template<typename ContextType>
        static void resize_chunk(ContextType& data);

        /**
         * @brief function responsible for transfering data from the packet in to the
         *        data structure
         */
        template<typename ContextType>
        static void deserialise_packet(ContextType& context, PacketInspector const&);

        /**
         * @brief perform operations to compenste for a missing packet
         * @details currently this does nothing, but this is expected to change
         */
        template<typename ContextType>
        static void process_missing_slice(ContextType& context);

        static void packet_stats(uint64_t packets_received, uint64_t packets_expected);

};

typedef BeamFormerDataTraits<PssLowTraits> BeamFormerDataTraitsLow;

} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/producers/rcpt_low/detail/BeamFormerDataTraits.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_BEAMFORMERDATATRAITS_H
