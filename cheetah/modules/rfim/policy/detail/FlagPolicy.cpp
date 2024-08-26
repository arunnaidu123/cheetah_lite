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
#include "cheetah/modules/rfim/policy/FlagPolicy.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {


template<typename TimeFrequencyType>
FlagPolicy<TimeFrequencyType>::FlagPolicy()
{
}

template<typename TimeFrequencyType>
FlagPolicy<TimeFrequencyType>::~FlagPolicy()
{
}

template<typename TimeFrequencyType>
typename FlagPolicy<TimeFrequencyType>::Adapter FlagPolicy<TimeFrequencyType>::adapter(TimeFrequencyType& data) const
{
    return Adapter(data);
}

template<typename TimeFrequencyType>
inline typename FlagPolicyAdapter<TimeFrequencyType>::ReturnType FlagPolicyAdapter<TimeFrequencyType>::data()
{
    return _data;
}

template<typename TimeFrequencyType>
FlagPolicyAdapter<TimeFrequencyType>::FlagPolicyAdapter(TimeFrequencyType& data)
    : _data(new FlaggedDataType(data))
    , _flag_data(_data->rfi_flags())
{
    std::fill(_flag_data.begin(), _flag_data.end(), false);
}

template<typename TimeFrequencyType>
FlagPolicyAdapter<TimeFrequencyType>::~FlagPolicyAdapter()
{
}

template<typename TimeFrequencyType>
template<typename SliceType>
typename std::enable_if<pss::astrotypes::is_slice<SliceType>::value, void>::type
FlagPolicyAdapter<TimeFrequencyType>::mark_good(SliceType const&)
{
}

template<typename TimeFrequencyType>
void FlagPolicyAdapter<TimeFrequencyType>::mark_good(NumericalRep const&, data::DimensionIndex<data::Frequency>)
{
}

template<typename TimeFrequencyType>
template<typename SliceType>
typename std::enable_if<pss::astrotypes::is_slice<SliceType>::value, void>::type
FlagPolicyAdapter<TimeFrequencyType>::mark_bad(SliceType const& slice)
{
    auto it=_flag_data.overlay(slice); // produces the equivalent spectrum in the flag_data
    std::fill(it.begin(), it.end(), true);
}

template<typename TimeFrequencyType>
void FlagPolicyAdapter<TimeFrequencyType>::mark_bad(NumericalRep const& val, data::DimensionIndex<data::Frequency>)
{
    *(_flag_data.begin() + (&val -  &*_data->tf_data().begin())) = true;
}

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
