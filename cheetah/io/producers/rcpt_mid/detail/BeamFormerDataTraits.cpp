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
#include "cheetah/io/producers/rcpt_mid/BeamFormerDataTraits.h"
#include <panda/Log.h>
#include <algorithm>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

template<class Traits>
BeamFormerDataTraits<Traits>::BeamFormerDataTraits()
{
}

template<class Traits>
BeamFormerDataTraits<Traits>::~BeamFormerDataTraits()
{
}

template<class Traits>
uint64_t BeamFormerDataTraits<Traits>::sequence_number(PacketInspector const& packet)
{
    return packet.sequence_number();
}

template<class Traits>
std::size_t BeamFormerDataTraits<Traits>::chunk_size(DataType const& chunk)
{
    return chunk.number_of_spectra() * chunk.number_of_channels();
}

template<class Traits>
std::size_t BeamFormerDataTraits<Traits>::packet_size()
{
    return ((PacketInspector::Packet::number_of_samples()*PacketInspector::Packet::number_of_channels()));
}

template<class Traits>
std::size_t BeamFormerDataTraits<Traits>::data_size()
{
    return PacketInspector::Packet::data_size();
}

template<class Traits>
std::size_t BeamFormerDataTraits<Traits>::packet_alignment_offset(PacketInspector const& inspector)
{
    return (inspector.packet().first_channel_number()/(PssMidTraits::number_of_channels/inspector.packet().number_of_channels()));
}

template<class Traits>
void BeamFormerDataTraits<Traits>::packet_stats(uint64_t packets_missing, uint64_t packets_expected)
{
    if(packets_missing > 0)
    {
        PANDA_LOG_WARN << "missing packets: " << packets_missing << " in " << packets_expected;
    }
}

template<class Traits>
template<typename ContextType>
void BeamFormerDataTraits<Traits>::deserialise_packet(ContextType& context, PacketInspector const& packet)
{
    unsigned offset = context.offset();
    auto packet_it=packet.packet().begin() + context.packet_offset();
    auto it=context.chunk().begin()+offset;
    panda::copy(packet_it, packet_it + context.size(), it);
}

template<class Traits>
template<typename ContextType>
void BeamFormerDataTraits<Traits>::process_missing_slice(ContextType& context)
{
    //essentially replacing the data corresponding to the missing packet to 0. (Dont know what is the best approch here.)
    unsigned offset = context.offset();
    auto it=context.chunk().begin() + offset;
    PANDA_LOG_DEBUG << "processing missing packet: data=" << (void*)&*it << context;
    for(std::size_t i=0; i < context.size() ; ++i) {
        *it = 0;
        assert(it!=context.chunk().end());
        ++it;
    }
}

template<class Traits>
template<typename ResizeContextType>
void BeamFormerDataTraits<Traits>::resize_chunk(ResizeContextType& context)
{
    PANDA_LOG_DEBUG << "resizing data: " << context;
    // drops any incomplete spectra
    context.chunk().resize(data::DimensionSize<data::Time>(context.size()/context.chunk().number_of_channels()));
}

template<class Traits>
constexpr typename BeamFormerDataTraits<Traits>::PacketNumberType
BeamFormerDataTraits<Traits>::max_sequence_number()
{
    return PacketInspector::Packet::max_sequence_number();
}

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
