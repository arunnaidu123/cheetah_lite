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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_UDPSTREAMFREQUENCYTIME_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_UDPSTREAMFREQUENCYTIME_H

#include "cheetah/io/producers/rcpt_low/Config.h"
#include "cheetah/io/producers/rcpt_low/BeamFormerDataTraits.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include <panda/PacketStream.h>
#include <panda/ResourceManager.h>
#include <boost/units/systems/si/time.hpp>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {

/**
 * @brief
 *    The UDP stream from the BeamFormer
 *
 * @details
 * The stream maintains a single thread to fill packets which are then delegated to
 * any number of other threads for processing.
 *
 */

template<typename Producer>
class UdpStreamFrequencyTimeTmpl : public panda::PacketStream<UdpStreamFrequencyTimeTmpl<Producer>, panda::ConnectionTraits<panda::Udp>, BeamFormerDataTraitsLow, Producer>
{
    protected:
        typedef panda::ConnectionTraits<panda::Udp> ConnectionTraits;

    private:
        typedef panda::PacketStream<UdpStreamFrequencyTimeTmpl<Producer>, ConnectionTraits, BeamFormerDataTraitsLow, Producer> BaseT;
        typedef BeamFormerDataTraitsLow::PacketInspector PacketInspector;

    public:
        typedef typename ConnectionTraits::EndPointType EndPointType;
        typedef boost::units::quantity<ska::cheetah::data::MegaHertz, double> FrequencyType;
        typedef boost::units::quantity<ska::cheetah::data::Seconds, double> TsampType;
        typedef typename BaseT::DataType::TimeType TimeType;

    public:
        UdpStreamFrequencyTimeTmpl(Config const& config);
        ~UdpStreamFrequencyTimeTmpl();

        /**
         * @brief post construction initialisation (called by Producer)
         */
        void init();

        /**
         * @brief initialise the chunk
         */
        template<typename DataType>
        std::shared_ptr<DataType> get_chunk(unsigned sequence_number, PacketInspector const& p);

    private:
        data::DimensionSize<data::Frequency> _n_channels;
        data::DimensionSize<data::Time> _n_samples;
        unsigned _n_time_samples_per_packet;
        unsigned _n_channels_per_packet;
        TsampType _tsamp;
        static ska::cheetah::utils::ModifiedJulianClock::time_point _tstart;
};

class UdpStreamFrequencyTime : public UdpStreamFrequencyTimeTmpl<UdpStreamFrequencyTime>
{
    public:
        UdpStreamFrequencyTime(Config const& config);
        ~UdpStreamFrequencyTime();

};

} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/producers/rcpt_low/detail/UdpStreamFrequencyTime.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_UDPSTREAMFREQUENCYTIME_H
