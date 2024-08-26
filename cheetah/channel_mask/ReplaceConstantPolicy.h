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
#ifndef SKA_CHEETAH_CHANNEL_MASK_REPLACECONSTANTPOLICY_H
#define SKA_CHEETAH_CHANNEL_MASK_REPLACECONSTANTPOLICY_H

#include "cheetah/channel_mask/ReplaceConstantPolicyConfig.h"

namespace ska {
namespace cheetah {
namespace channel_mask {

/**
 * @brief
 * @details
 */

template<typename T>
class ReplaceConstantPolicy
{
    public:
        ReplaceConstantPolicy(ReplaceConstantPolicyConfig<T> const& config);
        ~ReplaceConstantPolicy();

        /// replace all channels in the chunks with a constant value
        template<typename TimeFrequencyType>
        void operator()(TimeFrequencyType& data);

        // the configuration
        ReplaceConstantPolicyConfig<T> const& config() const;

    private:
        ReplaceConstantPolicyConfig<T> const& _config;
};


} // namespace channel_mask
} // namespace cheetah
} // namespace ska
#include "detail/ReplaceConstantPolicy.cpp"

#endif // SKA_CHEETAH_CHANNEL_MASK_REPLACECONSTANTPOLICY_H
