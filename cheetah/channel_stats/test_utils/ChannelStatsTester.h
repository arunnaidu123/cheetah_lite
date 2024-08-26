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
#ifndef SKA_CHEETAH_CHANNEL_STATS_TEST_UTILS_CHANNELSTATSTESTER_H
#define SKA_CHEETAH_CHANNEL_STATS_TEST_UTILS_CHANNELSTATSTESTER_H

#include "cheetah/utils/test_utils/AlgorithmTester.h"
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace channel_stats {
namespace test_utils {

/**
 * @brief
 * Generic functional test for the ChannelStats algorithms
 *
 * @details
 * To use you will first need to create suitable Traits descriptions of the algorithm and the
 * hardware requiremnst it needs to run.
 *
 *  e.g.
 * @code
 * struct MyAlgoTraits1 : public ChannelStatsTesterTraits<ResourceCapabilities, Architecture> {
 *      /// wrapper to execute the algorithm on requested provided device
 *      /// @return A dataype matching the tester requirements to verify the results
 *      ResultsType apply_algorithm(DeviceType&) const;
 * };
 * @endcode
 * Instantiate your algorithm tests by constructing suitable @class AlgorithmTestTraits classes
 * and then instantiate them with the INSTANTIATE_TYPED_TEST_SUITE_P macro
 *
 *  e.g.
 * @code
 * typedef ::testing::Types<MyAlgoTraits1, MyAlgoTraits2> MyTypes;
 * INSTANTIATE_TYPED_TEST_SUITE_P(MyAlgo, ChannelStatsTester, MyTypes);
 * @endcode
 *  n.b. the INSTANTIATE_TYPED_TEST_SUITE_P must be in the same namespace as this class
 *
 */

template<typename ArchitectureTag, typename ArchitectureCapability>
struct ChannelStatsTesterTraits : public utils::test::AlgorithmTesterTraits<ArchitectureTag, ArchitectureCapability> {


template <typename TestTraits>
class ChannelStatsTester : public cheetah::utils::test::AlgorithmTester<TestTraits>
{
    public:
        /**
         * @brief constructor
         */
        ChannelStatsTester();

        /**
         * @brief copy constructor not available
         * @internal if you add a copy constructor please remember "rule of 5" and unit tests
         */
        ChannelStatsTester(ChannelStatsTester const&) = delete;

    private:
};

TYPED_TEST_SUITE_P(ChannelStatsTester);

} // namespace test_utils
} // namespace channel_stats
} // namespace cheetah
} // namespace ska
#include "cheetah/test_utils/detail/ChannelStatsTester.cpp"


#endif // SKA_CHEETAH_CHANNEL_STATS_TEST_UTILS_CHANNELSTATSTESTER_H
