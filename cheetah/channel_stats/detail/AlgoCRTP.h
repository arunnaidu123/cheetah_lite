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
#ifndef SKA_CHEETAH_CHANNEL_STATS_ALGOCRTP_H
#define SKA_CHEETAH_CHANNEL_STATS_ALGOCRTP_H


namespace ska {
namespace cheetah {
namespace channel_stats {

/**
 * @brief
 * @details
 */
template<class DerivedT, class ArchT>
class AlgoCRTP
{
    public:
        /**
         * @brief constructor
         */
        AlgoCRTP();

        /**
         * @brief copy constructor not available
         * @internal if you add a copy constructor please remember "rule of 5" and unit tests
         */
        AlgoCRTP(AlgoCRTP const&) = delete;

    private:
};


} // namespace channel_stats
} // namespace cheetah
} // namespace ska

#include "cheetah/channel_stats/detail/AlgoCRTP.cpp"

#endif // SKA_CHEETAH_CHANNEL_STATS_ALGOCRTP_H
