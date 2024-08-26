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
#include "cheetah/channel_mask/ReplaceConstantPolicy.h"
#include <algorithm>


namespace ska {
namespace cheetah {
namespace channel_mask {


template<typename T>
ReplaceConstantPolicy<T>::ReplaceConstantPolicy(ReplaceConstantPolicyConfig<T> const& config)
    : _config(config)
{
}

template<typename T>
ReplaceConstantPolicy<T>::~ReplaceConstantPolicy()
{
}

template<typename T>
template<typename TimeFrequencySliceType>
inline void ReplaceConstantPolicy<T>::operator()(TimeFrequencySliceType& data)
{
    std::fill(data.begin(), data.end(), _config.value());
}

template<typename T>
ReplaceConstantPolicyConfig<T> const& ReplaceConstantPolicy<T>::config() const
{
    return _config;
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
