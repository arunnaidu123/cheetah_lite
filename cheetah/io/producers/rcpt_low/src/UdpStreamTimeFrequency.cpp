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
#include "cheetah/io/producers/rcpt_low/UdpStreamTimeFrequency.h"


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {


UdpStreamTimeFrequency::UdpStreamTimeFrequency(Config const& config)
:   _stream(config)
,   _ft_data_manager(_stream)
{
}

UdpStreamTimeFrequency::~UdpStreamTimeFrequency()
{
}

bool UdpStreamTimeFrequency::process()
{
    this->template get_chunk<TimeFrequencyType>(*std::get<0>(_ft_data_manager.next()));
    return false;
}

typename UdpStreamTimeFrequency::EndPointType UdpStreamTimeFrequency::local_end_point() const
{
    return _stream.local_end_point();
}
} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
