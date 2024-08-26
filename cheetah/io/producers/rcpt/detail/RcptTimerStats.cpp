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
#include "cheetah/io/producers/rcpt/RcptTimerStats.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {


template<typename ClockType>
RcptTimerStats<ClockType>::RcptTimerStats(std::size_t window_size)
    : _data_rates(boost::accumulators::tag::rolling_window::window_size=window_size)
    , _window_size(window_size)
{
    reset();
}

template<typename ClockType>
RcptTimerStats<ClockType>::~RcptTimerStats()
{
}

template<typename ClockType>
template<typename DataT>
void RcptTimerStats<ClockType>::operator()(DataT const& data, TimePointType time)
{
    auto delta = std::chrono::duration_cast<std::chrono::duration<double>>(time - _last);
    double data_rate = (data.data_size() * sizeof(typename DataT::DataType))/delta.count();
    _data_rates(data_rate);
    _last = ClockType::now();
}

template<typename ClockType>
void RcptTimerStats<ClockType>::reset()
{
    _last=ClockType::now();
}

template<typename ClockType>
panda::DataRate<double> RcptTimerStats<ClockType>::data_rate() const
{
    return panda::DataRate<double>(boost::accumulators::rolling_mean(_data_rates) * panda::bytes_per_second);
}

template<typename StreamType, typename ClockType>
StreamType& operator<<(StreamType& os, RcptTimerStats<ClockType> const& stats)
{
    os << stats.data_rate() << "\n";
    return os;
}

} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
