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
#ifndef SKA_CHEETAH_DATA_TIMEFREQUENCYSTATS_H
#define SKA_CHEETAH_DATA_TIMEFREQUENCYSTATS_H

#include "cheetah/data/ExtendedTimeFrequency.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A class to compute time-frequency stats: channel-wise and spectrum-wise mean and variance
 */
template<typename TimeFrequencyType>
class TimeFrequencyStats : public ExtendedTimeFrequency<TimeFrequencyType>
{
    private:
        typedef ExtendedTimeFrequency<TimeFrequencyType> BaseT;
        typedef typename BaseT::NumericalRep NumericalRep;

    public:
        /**
         * @brief A simple container class to hold the statistics of a
         * time sample (i.e. spectrum) or frequency channel
         */
        class Statistics {
            friend class TimeFrequencyStats<TimeFrequencyType>;

            public:
                Statistics();
                float mean() const;
                float variance() const;
                float stddev() const;

            private:
                Statistics(float m, float v);

            private:
                float _mean;
                float _variance;
        };

    public:
        typedef std::vector<Statistics> ReturnType;

    public:
        /**
         * @brief construct an object with median and variance statistics for each channel
         * and each spectrum.
         */
        TimeFrequencyStats(std::shared_ptr<TimeFrequencyType> const&);
        ~TimeFrequencyStats();

        /**
         * @brief return the Statistics (mean, variance, stddev) for each channel in the block
         */
        ReturnType const& channel_stats() const;

        /**
         * @brief return the Statistics (mean, variance, stddev) for each spectrum in the block
         */
        ReturnType const& spectrum_stats() const;

    private:
        template<typename T, typename Enable=void>
        struct RecomputeHelper;
        template<typename T>
        struct RecomputeHelper<T, EnableIfIsFrequencyTime<T, void>>;

        void recompute_inner_stats(ReturnType&, std::size_t inner_dim) const;
        void recompute_outer_stats(ReturnType&, std::size_t inner_dim, std::size_t outer_dim) const;

    private:
        mutable ReturnType _channel_stats;
        mutable ReturnType _spectrum_stats;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/TimeFrequencyStats.cpp"
#endif // SKA_CHEETAH_DATA_TIMEFREQUENCYSTATS_H
