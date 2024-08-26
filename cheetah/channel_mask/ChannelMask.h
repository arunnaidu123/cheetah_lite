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
#ifndef SKA_CHEETAH_CHANNEL_MASK_CHANNELMASK_H
#define SKA_CHEETAH_CHANNEL_MASK_CHANNELMASK_H

#include "cheetah/channel_mask/ReplaceConstantPolicy.h"
#include "cheetah/channel_mask/Config.h"


namespace ska {
namespace cheetah {
namespace channel_mask {
class FlaggedChannel;
/**
 * @brief Apply a policy to channels specified in the Config
 * @details
 */

template<typename NumericalRep, typename PolicyType>
class ChannelMask
{
        typedef data::TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;

    public:
        template<typename... PolicyConfigs>
        ChannelMask(Config const& config, PolicyConfigs&&... policy_construction_args);
        ~ChannelMask();

        TimeFrequencyType& operator()(TimeFrequencyType& tf_block) const;
        TimeFrequencyType& operator()(TimeFrequencyType& tf_block, FlaggedChannels const&) const;

    protected:
        Config const& _config;
        mutable PolicyType _policy;
};

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
#include "detail/ChannelMask.cpp"

#endif // SKA_CHEETAH_CHANNEL_MASK_CHANNELMASK_H
