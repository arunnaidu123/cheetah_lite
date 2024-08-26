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
#include "cheetah/io/producers/rcpt_low/PacketGeneratorConfig.h"
#include "cheetah/io/producers/rcpt_low/BeamFormerPacketInspector.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {


PacketGeneratorConfig::PacketGeneratorConfig()
    : cheetah::utils::Config("ska_low")
    , _number_of_channels(PssLowTraits::number_of_channels)
    , _number_of_spectra(BeamFormerPacketLow::number_of_samples())
    , _bandwidth(100.0*data::megahertz)
    , _fch1(350.0*data::megahertz)
    , _number_of_channels_per_packet(BeamFormerPacketLow::number_of_channels())
    , _interval(std::chrono::microseconds((unsigned)((double)_number_of_channels/static_cast<PacketGeneratorConfig::FrequencyType>(_bandwidth).value()*_number_of_spectra/(_number_of_channels/_number_of_channels_per_packet))))
{
}

PacketGeneratorConfig::~PacketGeneratorConfig()
{
}

void PacketGeneratorConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("interval", boost::program_options::value<std::size_t>()->default_value(_interval.count())
        ->notifier([this](std::size_t interval)
                   {
                       _interval = std::chrono::microseconds(interval);
                   }), "time seperation between packets in microseconds");
}

const data::DimensionSize<data::Frequency>& PacketGeneratorConfig::number_of_channels() const
{
    return _number_of_channels;
}

const data::DimensionSize<data::Time>& PacketGeneratorConfig::number_of_spectra() const
{
    return _number_of_spectra;
}

const std::chrono::microseconds& PacketGeneratorConfig::interval() const
{
    return _interval;
}

void PacketGeneratorConfig::interval(std::chrono::microseconds value)
{
    _interval = value;
}

const PacketGeneratorConfig::FrequencyType& PacketGeneratorConfig::bandwidth() const
{
    return _bandwidth;
}

const PacketGeneratorConfig::FrequencyType& PacketGeneratorConfig::fch1() const
{
    return _fch1;
}

const std::size_t& PacketGeneratorConfig::number_of_channels_per_packet() const
{
    return _number_of_channels_per_packet;
}

} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
