/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#include "cheetah/data/TimeFrequencyStats.h"
#include <utility>
#include <algorithm>
#include <cmath>
#include <numeric>


namespace ska {
namespace cheetah {
namespace data {

template<typename TimeFrequencyType>
TimeFrequencyStats<TimeFrequencyType>::Statistics::Statistics()
    : _mean(0.0)
    , _variance(0.0)
{
}

template<typename TimeFrequencyType>
TimeFrequencyStats<TimeFrequencyType>::Statistics::Statistics(float m, float v)
    : _mean(m)
    , _variance(v)
{
}

template<typename TimeFrequencyType>
inline float TimeFrequencyStats<TimeFrequencyType>::Statistics::mean() const
{
    return _mean;
}

template<typename TimeFrequencyType>
inline float TimeFrequencyStats<TimeFrequencyType>::Statistics::variance() const
{
    return _variance;
}

template<typename TimeFrequencyType>
inline float TimeFrequencyStats<TimeFrequencyType>::Statistics::stddev() const
{
    return std::sqrt(_variance);
}

template<typename TimeFrequencyType>
TimeFrequencyStats<TimeFrequencyType>::TimeFrequencyStats(std::shared_ptr<TimeFrequencyType> const& ptr)
    : BaseT(std::move(ptr))
{
}

template<typename TimeFrequencyType>
TimeFrequencyStats<TimeFrequencyType>::~TimeFrequencyStats()
{
}

template<typename TimeFrequencyType>
template<class T, typename Enable>
struct TimeFrequencyStats<TimeFrequencyType>::RecomputeHelper
{
    inline static void recompute_channel_stats(TimeFrequencyStats const& stats)
    {
        stats.recompute_inner_stats(stats._channel_stats, stats.number_of_channels());
    }
    inline static void recompute_spectrum_stats(TimeFrequencyStats const& stats)
    {
        stats.recompute_outer_stats(stats._spectrum_stats, stats.number_of_channels(), stats.number_of_spectra());
    }
};


template<typename TimeFrequencyType>
template<class T>
struct TimeFrequencyStats<TimeFrequencyType>::RecomputeHelper<T, EnableIfIsFrequencyTime<T, void>>
{
    inline static void recompute_channel_stats(TimeFrequencyStats const& stats)
    {
        stats.recompute_outer_stats(stats._channel_stats, stats.number_of_spectra(), stats.number_of_channels());
    }
    inline static void recompute_spectrum_stats(TimeFrequencyStats const& stats)
    {
        stats.recompute_inner_stats(stats._spectrum_stats, stats->number_of_spectra());
    }
};

template<typename TimeFrequencyType>
void TimeFrequencyStats<TimeFrequencyType>::recompute_inner_stats(ReturnType& stats, const std::size_t inner_dim) const
{
    typedef typename TimeFrequencyType::ConstIterator DataConstIt;
    const TimeFrequencyType& block = this->get();

    /** Here we use Welford's algorithm which:
     * - Provides decent numerical stability
     * - Allows calculation of both mean and variance in one pass
     * This implementation is based on:
     * https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
    */

    // Current mean, for every channel
    std::vector<float> mean(inner_dim, 0);

    // Current sum of squares of differences from the current mean, for every channel
    std::vector<float> accumulator(inner_dim, 0);

    // Samples processed
    size_t count = 0;
    DataConstIt it = block.begin();
    DataConstIt end_it = block.end();
    while (it != end_it)
    {
        ++count;
        const float invcount = 1.0 / count;
        for (size_t ichan = 0; ichan < inner_dim; ++ichan)
        {
            const float x = *it;
            const float diff = x - mean[ichan];
            mean[ichan] += diff * invcount;
            const float diff2 = x - mean[ichan];
            accumulator[ichan] += diff * diff2;
            ++it;
        }
    }

    // At the end of the loop, we have: variance = accumulator / count
    stats.resize(inner_dim);
    std::transform(
        mean.cbegin(), mean.cend(), accumulator.cbegin(), stats.begin(),
        [&count](float m, float a) {
            return Statistics(m, a / count);
        }
    );
}


template<typename TimeFrequencyType>
void TimeFrequencyStats<TimeFrequencyType>::recompute_outer_stats(ReturnType& stats, std::size_t inner_dim, std::size_t outer_dim) const
{
    const TimeFrequencyType& block = this->get();

    stats.resize(outer_dim);
    typename TimeFrequencyType::ConstIterator it = block.begin();
    typename TimeFrequencyType::ConstIterator const end_it = block.end();

    for (size_t isamp = 0; it != end_it; it += inner_dim, ++isamp)
    {
        /** Here we use the naive two-pass algorithm, because (at least with gcc <= 7)
         * it is 3x faster than Welford's and should achieve similar precision.
         * Doing two passes on the data is OK here because we're dealing with
         * a single spectrum which should fit in L2 cache,
         * or even L1 for small enough inner_dim.
         */
        const float mean = std::accumulate(it, it + inner_dim, 0.0f) / inner_dim;

        // Sum of squares of differences from mean
        const float sqrdiff_sum = std::accumulate(
            it, it + inner_dim, 0.0f,
            [&mean](float a, float x) {
                const float diff = x - mean;
                return a + diff * diff;
            }
        );

        stats[isamp] = Statistics(mean, sqrdiff_sum / inner_dim);
    }
}

template<typename TimeFrequencyType>
typename TimeFrequencyStats<TimeFrequencyType>::ReturnType const& TimeFrequencyStats<TimeFrequencyType>::channel_stats() const
{
    if (_channel_stats.size() != this->template dimension<data::Frequency>()) {
        RecomputeHelper<TimeFrequencyType>::recompute_channel_stats(*this);
    }
    return _channel_stats;
}

template<typename TimeFrequencyType>
typename TimeFrequencyStats<TimeFrequencyType>::ReturnType const& TimeFrequencyStats<TimeFrequencyType>::spectrum_stats() const
{
    if (_spectrum_stats.size() != this->template dimension<data::Time>()) {
        RecomputeHelper<TimeFrequencyStats>::recompute_spectrum_stats(*this);
    }
    return _spectrum_stats;
}

} // namespace data
} // namespace cheetah
} // namespace ska
