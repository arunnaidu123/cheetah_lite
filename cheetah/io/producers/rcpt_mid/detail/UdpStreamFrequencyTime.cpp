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
#include "cheetah/io/producers/rcpt_mid/UdpStreamFrequencyTime.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

template<typename Producer>
UdpStreamFrequencyTimeTmpl<Producer>::UdpStreamFrequencyTimeTmpl(Config const& config)
    : BaseT(config.engine(), ConnectionTraits::SocketType(static_cast<panda::Engine&>(config.engine()), config.remote_end_point()))
    , _n_channels(config.number_of_channels())
    , _n_samples(config.spectra_per_chunk())
{
    boost::asio::socket_base::receive_buffer_size option(16*1024*1024);
    this->connection().socket().set_option(option);
}

template<typename Producer>
UdpStreamFrequencyTimeTmpl<Producer>::~UdpStreamFrequencyTimeTmpl()
{
}

template<typename Producer>
void UdpStreamFrequencyTimeTmpl<Producer>::init()
{
    this->BaseT::init();
    this->start();
}


template<typename Producer>
template<typename DataType>
std::shared_ptr<DataType> UdpStreamFrequencyTimeTmpl<Producer>::get_chunk(unsigned , PacketInspector const& packet)
{
    auto chunk = BaseT::template get_chunk<DataType>();
    if(chunk) {
        chunk->resize( _n_samples, _n_channels);
    }
    const UdpStreamFrequencyTimeTmpl<Producer>::TsampType fraction = ((double)(packet.packet().timestamp_attoseconds()*1e-18)*ska::cheetah::data::seconds);
    const UdpStreamFrequencyTimeTmpl<Producer>::TsampType seconds = ((double)(packet.packet().timestamp_seconds())*ska::cheetah::data::seconds);
    const std::chrono::time_point<std::chrono::system_clock> system_epoch;
    chunk->start_time(ska::cheetah::utils::ModifiedJulianClock::time_point(system_epoch+seconds+fraction));

    UdpStreamFrequencyTimeTmpl<Producer>::FrequencyType fch1 = packet.packet().first_channel_frequency() * data::megahertz;
    UdpStreamFrequencyTimeTmpl<Producer>::FrequencyType bandwidth = (((double)std::ceil((packet.packet().channel_separation()*1e-9)*PssMidTraits::number_of_channels))*boost::units::si::mega * ska::cheetah::data::hertz);
    UdpStreamFrequencyTimeTmpl<Producer>::FrequencyType foff = (-1.0*static_cast<UdpStreamFrequencyTimeTmpl<Producer>::FrequencyType>(bandwidth).value()/(double)PssMidTraits::number_of_channels)*boost::units::si::mega * ska::cheetah::data::hertz;
    chunk->sample_interval(TimeType(4*((double)PssMidTraits::number_of_channels/static_cast<UdpStreamFrequencyTimeTmpl<Producer>::FrequencyType>(bandwidth).value())*ska::cheetah::data::microseconds));
    chunk->set_channel_frequencies_const_width(fch1, foff);
    return chunk;
}


} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
