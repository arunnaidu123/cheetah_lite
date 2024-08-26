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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_RCPTTIMERSTATS_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_RCPTTIMERSTATS_H
#include "panda/DataRate.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {

/**
 * @brief
 *    Calculate rcpt specific performance information
 * @details
 *
 */

template<typename ClockType>
class RcptTimerStats
{
    private:
        typedef typename ClockType::time_point TimePointType;
        typedef typename ClockType::duration TimeDurationType;
        typedef boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::rolling_mean> > RateAccumulatorType;

    public:

        RcptTimerStats(std::size_t window_size = 200);
        ~RcptTimerStats();

        /**
         * @brief add a new datapoint
         *        n.b. this is not thread safe.
         */
        template<typename DataT>
        void operator()(DataT const&, TimePointType);

        /**
         * @brief return the average data rate
         */
        panda::DataRate<double> data_rate() const;

        /**
         * @brief reset all counters and set the first timestamp to now
         */
        void reset();

    private:
        TimePointType _last;
        RateAccumulatorType _data_rates;
        const std::size_t _window_size;
};

/**
 * @brief  write a summary of the RcptTimerStats performance metrics to the stream
 */
template<typename StreamType, typename ClockType>
StreamType& operator<<(StreamType& os, RcptTimerStats<ClockType> const&);

} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#include "cheetah/io/producers/rcpt/detail/RcptTimerStats.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_RCPTTIMERSTATS_H
