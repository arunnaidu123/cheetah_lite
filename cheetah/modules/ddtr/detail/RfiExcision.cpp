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
#include "cheetah/modules/ddtr/detail/RfiExcision.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename TimeFrequencyType, typename RfiFlagDataType>
RfiExcision<TimeFrequencyType, RfiFlagDataType>::RfiExcision( RfiFlagDataType& data, size_t offset, float ideal_rms, float threshold, bool active)
    : _flag_it(data.rfi_flags().begin() + offset)
    , _tf_it(data.tf_data().begin() + offset)
    , _wrapindex(0)
    , _ideal_rms(ideal_rms)
    , _threshold(threshold)
    , _active(active)
    , _tf_end_it(data.tf_data().cend())
{
    assert(data.rfi_flags().data_size() == data.tf_data().data_size());
    _noise.resize(8192); // A vector to save pre calculated random numbers.
                         // The size is fixed such that it s not too big to waste resources or too small to reduce the 'randomness' of the replacement values

    // we take the median of the means for each channel as our default replacement value
    typedef typename RfiFlagDataType::Statistics Statistics;
    auto channel_stats = data.channel_stats();
    auto subband_nchans = channel_stats.size() / 8; // Dividing the band into 8 subbands so as to choose the best median from the middle of the band
    std::nth_element(channel_stats.begin() + 2*subband_nchans, channel_stats.begin() + (std::size_t)(2.5)*subband_nchans, channel_stats.begin() + 3*subband_nchans - 1,
        [](Statistics const& s1, Statistics const& s2)
        {
            return s1.mean() < s2.mean();
        } );
    _replacement_value = channel_stats[(std::size_t)((2.5)*subband_nchans)].mean();

    /* Setting  up  the random generator */
    static std::random_device rd;
    std::seed_seq seed {  rd(), rd(), rd(), rd(), rd() };
    auto e2 = std::mt19937( seed );
    auto distribution = std::normal_distribution<float>(_replacement_value,_ideal_rms);
    std::generate(_noise.begin(),_noise.end(), std::bind(distribution, e2));
    _value = _noise[0];
}

template<typename TimeFrequencyType, typename RfiFlagDataType>
RfiExcision<TimeFrequencyType, RfiFlagDataType>::~RfiExcision()
{
}

template<typename TimeFrequencyType, typename RfiFlagDataType>
bool RfiExcision<TimeFrequencyType, RfiFlagDataType>::operator!=( RfiExcision<TimeFrequencyType, RfiFlagDataType> const& iterator) const
{
    return this->_tf_it != iterator._tf_it;
}

template<typename TimeFrequencyType, typename RfiFlagDataType>
bool RfiExcision<TimeFrequencyType, RfiFlagDataType>::operator==( RfiExcision<TimeFrequencyType, RfiFlagDataType> const& iterator) const
{
    return this->_tf_it == iterator._tf_it;
}

template<typename TimeFrequencyType, typename RfiFlagDataType>
RfiExcision<TimeFrequencyType, RfiFlagDataType>& RfiExcision<TimeFrequencyType, RfiFlagDataType>::operator++()
{
    ++_tf_it;
    ++_flag_it;
    ++_wrapindex;
    if(_tf_it!=_tf_end_it)
    {
        bool do_replacement;
        if (_active)
            do_replacement = (*(_flag_it)) || (std::fabs((float) *(_tf_it) - _replacement_value) >= _threshold *_ideal_rms);
        else
            do_replacement = (*(_flag_it));

        if (do_replacement)
        {
            if (_wrapindex >= _noise.size())
            {
                _wrapindex = 0;
            }
            _value = static_cast<NumericalRep>(_noise[_wrapindex]);
        }
        else
            _value = *(_tf_it);

    }
    return *this;
}

template<typename TimeFrequencyType, typename RfiFlagDataType>
typename RfiExcision<TimeFrequencyType, RfiFlagDataType>::const_reference RfiExcision<TimeFrequencyType, RfiFlagDataType>::operator*() const
{
    return _value;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
