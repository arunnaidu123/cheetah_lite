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
#include "cheetah/modules/rfim/iqrmcpu/detail/IqrmImpl.h"
#include "panda/Error.h"
#include <boost/algorithm/clamp.hpp>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {

template <typename InputIt, typename OutputIt>
void IqrmImpl::iqrm_mask(InputIt begin, InputIt end, OutputIt dest) const
{
    typedef typename std::iterator_traits<InputIt>::value_type T;
    typedef DifferenceType<InputIt> DifferenceT;
    const DifferenceT nchan = std::distance(begin, end);
    if (nchan <= 0)
        return;
    else if (nchan == 1)
    {
        *dest = false;
        return;
    }
    const double thr = _config.threshold();
    std::fill(dest, dest + nchan, false);
    std::vector<T> diff(nchan);
    VoteTracker<DifferenceT> tracker(nchan);

    for (DifferenceT lag: trial_lags(nchan))
    {
        lagged_diff(begin, end, lag, diff.begin());
        const RobustStats stats = robust_stats(diff.begin(), diff.end());
        for (DifferenceT ii = 0; ii < nchan; ++ii)
        {
            const bool outlier = (diff[ii] - stats.median) > thr * stats.std;
            if (outlier)
            {
                // cf. definition of lagged diff
                // Here the difference between the value at index ii and that at index ii - lag
                // is deemed abonormally large
                const DifferenceT caster = boost::algorithm::clamp(ii - lag, 0, nchan - 1);
                tracker.add_vote(caster, ii);
            }
        }
    }

    // Write result
    for (DifferenceT ii = 0; ii < nchan; ++ii)
        if (tracker.is_outlier(ii))
            *(dest + ii) = true;

    // Mask edge_channels forcibly on each side
    const DifferenceT ec = boost::algorithm::clamp(DifferenceT(_config.edge_channels()), 0, nchan);
    std::fill(dest, dest + ec, true);
    std::fill(dest + nchan - ec, dest + nchan, true);
}


template <typename InputIt, typename OutputIt>
void IqrmImpl::lagged_diff(InputIt begin, InputIt end, DifferenceType<InputIt> lag, OutputIt dest) const
{
    typedef typename std::iterator_traits<InputIt>::value_type T;
    const DifferenceType<InputIt> n = std::distance(begin, end);
    if (n <= 0)
        return;
    lag = boost::algorithm::clamp(lag, -n, n);
    if (lag >= 0)
    {
        const T first = *begin;
        std::transform(
            begin, begin + lag, dest,
            [&first](T val) {
                return val - first;
            }
        );
        std::transform(begin + lag, end, begin, dest + lag, std::minus<T>{});
    }
    else
    {
        const T last = *(end - 1);
        // NOTE: lag is negative here
        std::transform(begin, end + lag, begin - lag, dest, std::minus<T>{});
        std::transform(end + lag, end, dest + n + lag,
            [&last](T val) {
                return val - last;
            }
        );
    }
}

template <typename InputIt>
IqrmImpl::RobustStats IqrmImpl::robust_stats(InputIt begin, InputIt end) const
{
    typedef typename std::iterator_traits<InputIt>::value_type T;
    std::vector<T> vec(begin, end);
    const size_t n = vec.size();

    const auto quantile = [&vec, &n](double q) {
        const double x = q * (n - 1); // X coordinate of desired quantile in array
        // Desired quantile would be between array indices a and b if the array was sorted
        const size_t a = floor(x);
        const size_t b = ceil(x);
        // Now: find out the vector elements va and vb that are ranked a-th and b-th
        // This places the b-th largest element in b-th position
        // and guarantees that all elements before that are smaller.
        std::nth_element(vec.begin(), vec.begin() + b, vec.end());
        const double vb = vec[b];
        // So now we can afford to call n-th element just on the range [0, b[
        std::nth_element(vec.begin(), vec.begin() + a, vec.begin() + b);
        const double va = vec[a];
        // Linearly interpolate between va and vb
        const double h = x - a;
        return (1.0 - h) * va + h * vb;
    };

    const double q1 = quantile(0.25); // First quartile
    const double q2 = quantile(0.50); // Second quartile, i.e. median
    const double q3 = quantile(0.75); // Third quartile

    // Estimate stddev of the range using the formula: sigma = IQR / 1.349,
    // where IQR denotes the inter-quartile range.
    // This formula implicitly assumes Gaussian distributed data.
    return IqrmImpl::RobustStats(q2, (q3 - q1) / 1.349);
}

template <typename DiffT>
std::vector<DiffT> IqrmImpl::trial_lags(DiffT nchan) const
{
    std::vector<DiffT> lags;
    const DiffT radius = boost::algorithm::clamp(_config.radius(), 1, nchan);
    DiffT lag = 1;
    while (lag <= radius)
    {
        lags.push_back(lag);
        lags.push_back(-lag);
        lag = std::max(static_cast<DiffT>(1.5 * lag), lag + 1);
    }
    return lags;
}

template <typename DiffT>
IqrmImpl::VoteTracker<DiffT>::VoteTracker(DiffT nchan)
    : _cast(nchan)
    , _received(nchan)
{

}

template <typename DiffT>
void IqrmImpl::VoteTracker<DiffT>::add_vote(DiffT caster, DiffT recipient)
{
    _cast[caster].insert(recipient);
    _received[recipient].insert(caster);
}

template <typename DiffT>
bool IqrmImpl::VoteTracker<DiffT>::is_outlier(DiffT i) const
{
    for (DiffT j : _received[i])
    {
        // i is an outlier if j has cast strictly less votes than i has received
        if (_cast[j].size() < _received[i].size())
            return true;
    }
    return false;
}


} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska