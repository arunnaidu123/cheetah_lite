/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#include "cheetah/modules/rfim/FlaggedDataReplacer.h"
#include "cheetah/data/RfimFlaggedData.h"
#include <algorithm>

namespace ska{
namespace cheetah{
namespace modules{
namespace rfim{

template<typename TimeFrequencyType>
FlaggedDataReplacer<TimeFrequencyType>::FlaggedDataReplacer()
{
}

template<typename TimeFrequencyType>
FlaggedDataReplacer<TimeFrequencyType>::~FlaggedDataReplacer()
{
}

template<typename TimeFrequencyType>
void FlaggedDataReplacer<TimeFrequencyType>::update_stats( TimeFrequencyType& data)
{
    // First step during instantiation of the object is to convert the data from TimeFrequencyType to RfimFlaggedData
    // This lets us use the Statistics module to calculate and store the channel_stats
    typedef typename data::RfimFlaggedData<TimeFrequencyType>::Statistics Statistics;
    data::RfimFlaggedData<TimeFrequencyType> flagged_data = data::RfimFlaggedData(data);
    auto channel_stats = flagged_data.channel_stats();

    //getting parameters to generate gaussian noise
    _mean = channel_stats[0].mean();        //Using Mean and SD of first channel
    _rms = channel_stats[0].stddev();

    //setting up the noise
    static std::random_device rd;
    std::seed_seq seed {rd(), rd(), rd(), rd(), rd()};
    auto e2 = std::minstd_rand(seed);
    auto distribution = std::normal_distribution<float>(_mean,_rms);
    std::generate(_noise.begin(),_noise.end(), std::bind(distribution, e2));
    _value = _noise[0];
}

template<typename TimeFrequencyType>
FlaggedDataReplacer<TimeFrequencyType>& FlaggedDataReplacer<TimeFrequencyType>::operator++()
{
    ++_wrapindex;       //incrementing the pointer that would indicate to a location in the _noise array
    if(_wrapindex >= _noise.size())
    {
        //wrapping the _noise by simpling setting value of the counter to 0
        _wrapindex = 0;
    }
    _value = static_cast<NumericalRep>(_noise[_wrapindex]);     //doing appropriate casting
    return *this;
}

template< typename TimeFrequencyType>
typename FlaggedDataReplacer<TimeFrequencyType>::const_reference FlaggedDataReplacer<TimeFrequencyType>::operator*() const
{
    return _value;
}

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
