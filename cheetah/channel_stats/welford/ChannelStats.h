/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#ifndef SKA_CHEETAH_CHANNEL_STATS_WELFORD_CHANNELSTATS_H
#define SKA_CHEETAH_CHANNEL_STATS_WELFORD_CHANNELSTATS_H


namespace ska {
namespace cheetah {
namespace channel_stats {
namespace welford {

/**
 * @brief Calculating channel statistics using the Welford approximation
 * @details TODO
 */

class ChannelStats : public channel_stats::AlgoCRTP<ChannelStats, Cpu>
{
    public:
        typedef Cpu Architectrue;

    public:
        /**
         * @brief constructor
         */
        ChannelStats();

        /**
         * @brief copy constructor not available
         * @internal if you add a copy constructor please remember "rule of 5" and unit tests
         */
        ChannelStats(ChannelStats const&) = delete;

    private:
};


} // namespace welford
} // namespace channel_stats
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_CHANNEL_STATS_WELFORD_CHANNELSTATS_H
