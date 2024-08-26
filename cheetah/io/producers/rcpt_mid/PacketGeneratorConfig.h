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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_PACKETGENERATORCONFIG_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_PACKETGENERATORCONFIG_H

#include "cheetah/utils/Config.h"
#include "cheetah/io/producers/rcpt_mid/BeamFormerPacketInspector.h"
#include "cheetah/data/Units.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

/**
 * @brief Configuration for the packet generator to the time seperation of the packets
 */
class PacketGeneratorConfig : public cheetah::utils::Config
{
    public:
        typedef boost::units::quantity<ska::cheetah::data::MegaHertz, double> FrequencyType;

    public:
        PacketGeneratorConfig();
        ~PacketGeneratorConfig();

        /**
         * @brief number of channels per chunk
         */
        const data::DimensionSize<data::Frequency>& number_of_channels() const;
        void number_of_channels(const data::DimensionSize<data::Frequency>& value);

        /**
         * @brief number of spectra per packet
         */
        const data::DimensionSize<data::Time>& number_of_spectra() const;
        void number_of_spectra(const data::DimensionSize<data::Time>& value);

        /**
         * @brief time sepearation between consequetive packets in micro seconds
         */
        const std::chrono::microseconds& interval() const;
        void interval(std::chrono::microseconds value);

        /**
         * @brief returns the bandwidth in Megahertz
         */
        const FrequencyType& bandwidth() const;

        /**
         * @brief returns frequency of the highest channel.
         */
        const FrequencyType& fch1() const;

        /**
         * @brief return the number of channels per packet
         */
        const std::size_t& number_of_channels_per_packet() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        data::DimensionSize<data::Frequency> _number_of_channels;
        data::DimensionSize<data::Time> _number_of_spectra;
        FrequencyType _bandwidth;
        FrequencyType _fch1;
        std::size_t _number_of_channels_per_packet;
        std::chrono::microseconds _interval;
};

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_PACKETGENERATORCONFIG_H
