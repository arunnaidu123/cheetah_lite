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
#include "cheetah/io/producers/rcpt/UdpStream.h"
#include "cheetah/io/producers/rcpt/BeamFormerPacket.h"
#include "panda/Log.h"
#include <iostream>
#include <algorithm>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {

template<typename Producer>
UdpStreamTmpl<Producer>::UdpStreamTmpl(Config const& config)
    //: BaseT(config.engine(), ConnectionTraits::SocketType(config.engine(), config.local_end_point()))
    : BaseT(config.engine(), ConnectionTraits::SocketType(config.engine(), config.remote_end_point()))
    , _n_channels(config.number_of_channels())
    , _n_samples(config.samples_per_chunk())
{
}

template<typename Producer>
UdpStreamTmpl<Producer>::~UdpStreamTmpl()
{
}

template<typename Producer>
void UdpStreamTmpl<Producer>::init()
{
    this->BaseT::init();
    this->start();
}

template<typename Producer>
template<typename DataType>
std::shared_ptr<DataType> UdpStreamTmpl<Producer>::get_chunk(unsigned , BeamFormerPacketInspector const&)
{
    auto chunk = BaseT::template get_chunk<DataType>();
    PANDA_LOG_DEBUG << "creating new chunk (" << chunk.get() << ")";
    if(chunk) {
        chunk->resize( data::DimensionSize<data::Time>(_n_samples)
                     , data::DimensionSize<data::Frequency>(_n_channels));
    }
    return chunk;
}


} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
