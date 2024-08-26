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
#ifndef SKA_CHEETAH_MODULES_RFIM_IQRMCPU_DETAIL_IQRMIMPL_H
#define SKA_CHEETAH_MODULES_RFIM_IQRMCPU_DETAIL_IQRMIMPL_H

#include "cheetah/modules/rfim/iqrmcpu/Config.h"
#include <iterator>
#include <vector>
#include <set>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {

class IqrmImpl {
    private:
        template <typename Iterator>
        using DifferenceType = typename std::iterator_traits<Iterator>::difference_type;

    public:
        IqrmImpl(Config const& config);

        /**
         * @brief Compute the IQRM channel mask given a per-channel statistic of a TimeFrequency
         *  data block, and write the result in output range 'dest'. A value of 'true' means that
         *  the corresponding channel should be masked.
         *  The per-channel statistic can be anything that correlates well with RFI
         *  contamination (higher value = higher chance of being contaminated). On MeerKAT data
         *  for example, the per-channel standard deviation works very well.
         */
        template <typename InputIt, typename OutputIt>
        void iqrm_mask(InputIt begin, InputIt end, OutputIt dest) const;

        /**
         * @brief Compute the lagged difference of an input range and store it in another range
         * @details The result is defined as:
         *     diff[i] = input[i] - input[i - lag]
         * Where lag can be negative, and where out of bounds access to 'input' are handled as:
         * input[i] = input[0] for i < 0
         * input[i] = input[n-1] for i > n - 1
         */
        template <typename InputIt, typename OutputIt>
        void lagged_diff(InputIt begin, InputIt end, DifferenceType<InputIt> lag, OutputIt dest) const;

        /**
         * @brief A simple struct just to hold the results of robust_stats()
         */
        struct RobustStats
        {
            RobustStats(double m, double s);
            const double median;
            const double std;
        };

        /**
         * @brief Calculate the median and robust standard deviation of the data in given input
         * range. The stddev is taken to be IQR / 1.349, where IQR = inter-quartile range
         */
        template <typename InputIt>
        RobustStats robust_stats(InputIt begin, InputIt end) const;

    private:
        /**
         * @brief Return the sequence of trial lag values. nchan is passed as an argument so that
         * we can enforce radius <= nchan.
         * @details DiffT here is the signed integer type that is used to represent input data
         * iterator differences (usually ptrdiff_t)
         */
        template <typename DiffT>
        std::vector<DiffT> trial_lags(DiffT nchan) const;

        /**
         * @brief A helper class that implements the so-called voting system of IQRM
         * @details DiffT here is the signed integer type that is used to represent input data
         * iterator differences (usually ptrdiff_t)
         */
        template <typename DiffT>
        class VoteTracker {
            public:
                VoteTracker(DiffT nchan);
                void add_vote(DiffT caster, DiffT recipient);
                bool is_outlier(DiffT i) const;

            private:
                typedef typename std::vector<std::set<DiffT>> VoteTable;
                // cast[i] = set of channel indices against which channel i has cast a vote
                VoteTable _cast;
                // received[i] = set of channel indices that have cast a vote against i
                VoteTable _received;
        };

    protected:
        Config const& _config;
};

} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/rfim/iqrmcpu/detail/IqrmImpl.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_IQRMCPU_DETAIL_IQRMIMPL_H