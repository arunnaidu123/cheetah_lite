/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#include "cheetah/io/producers/rcpt/SkaUdpConfig.h"


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {


template<SkaSelector::Stream Stream>
auto SkaUdpConfig::rcpt_config() -> typename rcpt::SkaUdpStream<Stream>::Config&
{
    return RcptConfigHelper<Stream>::config(*this);
}

template<SkaSelector::Stream Stream>
auto SkaUdpConfig::rcpt_config() const -> typename rcpt::SkaUdpStream<Stream>::Config const&
{
    return RcptConfigHelper<Stream>::config(*this);
}

template<>
struct SkaUdpConfig::RcptConfigHelper<SkaSelector::Stream::SkaLow>
{
    typedef typename rcpt::SkaUdpStream<SkaSelector::Stream::SkaLow>::Config Type;

    static inline
    Type& config(SkaUdpConfig& p_config)
    {
        return p_config._rcpt_low_config;
    }

    static inline
    Type const& config(SkaUdpConfig const& p_config)
    {
        return p_config._rcpt_low_config;
    }
};

template<>
struct SkaUdpConfig::RcptConfigHelper<SkaSelector::Stream::SkaMid>
{
    typedef typename rcpt::SkaUdpStream<SkaSelector::Stream::SkaMid>::Config Type;

    static inline
    Type& config(SkaUdpConfig& p_config)
    {
        return p_config._rcpt_mid_config;
    }

    static inline
    Type const& config(SkaUdpConfig const& p_config)
    {
        return p_config._rcpt_mid_config;
    }
};

} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
