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
#include "cheetah/channel_mask/FlaggedChannels.h"


namespace ska {
namespace cheetah {
namespace channel_mask {

template<typename TimeFrequencyType>
std::vector<typename TimeFrequencyType::SliceType> FlaggedChannels::flagged_channels(TimeFrequencyType& tf_data) const
{
    std::vector<typename TimeFrequencyType::SliceType> r;
    for(auto span : _channel_ranges)
    {
        span.trim(tf_data.template dimension<data::Frequency>());
        if(span.span() != data::DimensionSize<data::Frequency>(0))
        {
            r.emplace_back(tf_data.slice(span));
        }
    }
    return r;
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
