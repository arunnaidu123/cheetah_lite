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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_UDPSTREAMTIMEFREQUENCY_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_UDPSTREAMTIMEFREQUENCY_H

#include "cheetah/io/producers/rcpt_low/Config.h"
#include "cheetah/io/producers/rcpt_low/UdpStreamFrequencyTime.h"
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
 * @brief SKA Low data stream producing TimeFrequency Datatype
 * @details for use within a panda::DataManager
 */

//typedef typename BeamFormerDataTraitsLow::DataType::value_type ValueType;
// FIXME - we want to be able to use the above expression, which we can only do if the main exe
//         can handle types other than uint8_t
typedef uint8_t ValueType;

class UdpStreamTimeFrequency : public panda::Producer<UdpStreamTimeFrequency, data::TimeFrequency<Cpu, ValueType>>
{
    public:
        typedef data::TimeFrequency<Cpu, ValueType> TimeFrequencyType;
        typedef typename UdpStreamFrequencyTime::EndPointType EndPointType;

    public:
        UdpStreamTimeFrequency(Config const& config);
        ~UdpStreamTimeFrequency();

        bool process();

        EndPointType local_end_point() const;

    private:
        UdpStreamFrequencyTime _stream;
        panda::DataManager<UdpStreamFrequencyTime> _ft_data_manager;
};


} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_UDPSTREAMTIMEFREQUENCY_H
