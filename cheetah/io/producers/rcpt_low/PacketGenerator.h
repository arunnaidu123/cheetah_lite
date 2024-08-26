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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_PACKETGENERATOR_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_PACKETGENERATOR_H
#include "cheetah/io/producers/rcpt_low/BeamFormerPacketInspector.h"
#include "cheetah/io/producers/rcpt_low/PacketGeneratorConfig.h"
#include "cheetah/data/TimeFrequency.h"
#include "panda/Buffer.h"
#include "panda/Engine.h"
#include <type_traits>
#include <vector>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {

/**
 * @brief
 *     Packs data into a UDP stream Packet Header format of the BeamFormer
 *
 * @details
 *
 */

template<class DataGenerator>
class PacketGenerator
{
    public:
        typedef int8_t SampleDataType;
        typedef DataGenerator ModelType;
        typedef data::FrequencyTime<Cpu, SampleDataType> DataType;
        typedef boost::units::quantity<ska::cheetah::data::Seconds, double> TimeType;
        typedef boost::units::quantity<ska::cheetah::data::MegaHertz, double> FrequencyType;

    public:
        PacketGenerator(DataGenerator& model, PacketGeneratorConfig const& config);

        ~PacketGenerator();

        /**
         * @brief send out a single packet
         * @param handler the handler to call when the packet has been sent
         */
        template<typename Handler>
        void send_one(Handler&& handler);

        /**
         * @brief returns next buffer
         */
        ska::panda::Buffer<char> next();

        /**
         * @brief The interval between packets in microsec.
         */
        std::chrono::microseconds interval() const;

        void abort();

    private:
        typedef BeamFormerPacketLow Packet;
        typedef typename BeamFormerPacketLow::PacketNumberType CounterType;

    private:
        DataGenerator& _model;                // The data generation model
        DataType _data;
        DataType::ConstIterator _data_iterator;
        CounterType _counter;
        std::chrono::microseconds _interval; // The interval between packets in microsec.
        const std::size_t _max_buffers;
        std::size_t _buffer_index;
        std::vector<ska::panda::Buffer<char>> _buffers;
        ska::panda::Engine _engine;
        ska::cheetah::utils::ModifiedJulianClock::time_point _timestamp;
        TimeType _tsamp;
        std::size_t _number_of_samples_per_packet;
        std::size_t _number_of_channels_per_packet;
        FrequencyType _bandwidth;
        FrequencyType _fch1;

};

} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/producers/rcpt_low/detail/PacketGenerator.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_PACKETGENERATOR_H
