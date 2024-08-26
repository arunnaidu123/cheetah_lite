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


FlaggedChannels::FlaggedChannels()
{
}

FlaggedChannels::~FlaggedChannels()
{
}

void FlaggedChannels::insert(pss::astrotypes::DimensionSpan<data::Frequency> const& span)
{
    // find where to slot this in
    auto it=std::find_if(_channel_ranges.begin(), _channel_ranges.end(),
                        [&](pss::astrotypes::DimensionSpan<data::Frequency> const& d) {
                            return d.start() <= span.start();
                        });
    if(it != _channel_ranges.cend())
    {
        pss::astrotypes::DimensionIndex<data::Frequency> const end = it->start() + it->span();
        if(end >= span.start()) {
            pss::astrotypes::DimensionIndex<data::Frequency> const span_end = span.start() + span.span();
            if(span_end > end) {
                it->span() += span_end - end;
            }
        }
        else {
           _channel_ranges.insert(it, span);
        }
    }
    else {
        _channel_ranges.push_back(span);
    }
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
