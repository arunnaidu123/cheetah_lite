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
#ifndef SKA_CHEETAH_MODULES_RFIM_TEST_RFIMTESTER_H
#define SKA_CHEETAH_MODULES_RFIM_TEST_RFIMTESTER_H

#include "cheetah/utils/test_utils/AlgorithmTester.h"
#include "cheetah/modules/rfim/Rfim.h"
#include "cheetah/modules/rfim/Metrics.h"
#include "cheetah/modules/rfim/policy/FlagPolicy.h"
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {

/**
 * @brief
 * Generic functional test for the Rfim algorithm
 *
 * --------- Operational Conditions ----------------------
 *
 *  F: Hardware Failure
 *  F2) Signal Empty across all channels
 *  F3) Signal Empty in a single channel
 *      @param channel
 *  F4) Signal Empty in a single block across more than one adjacent channel
 *     @param position of block
 *     @param number of channels
 *  F5) Signal Empty in multiple seperate blocks, each containing more than one channel
 *     @param number of blocks
 *     foreach block:
 *     @param position of block
 *     @param number of channels
 *
 *  P: External Sources Present
 *  P0) None - just noise
 *  P1) No Pulsars in signal
 *  P2) Regular Pulsar in signal
 *  P3) Multiple Pulsars in signal
 *  P4) Single Accelerated Pulsar in signal
 *  P5) Multiple Accelerated Pulsars in signal
 *  P6) Mixed regular pulsar and acelerated pulsar
 *
 *  R: RFI conditions
 *  R1) Single channel spike across multiple timesamples
 *      @param size of spike
 *  R2) Single channel spike in a single timesample
 *      @param size of spike
 *  R3) wide band spike across multiple channels in a single block
 *      @param size of spike
 *      @param size of block
 *      @param position of block
 *  R4) wide band spikes across multiple channels in multiple blocks of channels
 *      @param number of blocks
 *      foreach block:
 *      @param size of spike
 *      @param size of block
 *      @param position of block
 *
 *  Test Sceanarios:
 *  Test should be generated that combine each element from the sets F, P, and R, each with a
 *  representative array of parameters where appropriate. RFI should be detected without affecting
 *  the signal
 *
 * @details
 * To use you will first need to create suitable Traits descriptions of the algorithm and the
 * hardware requiremnst it needs to run.
 *
 *  e.g.
 * @code
 * template<typename NumericalRep>
 * struct MyAlgoTraits1 : public RfimTesterTraits<Architecture, ResourceCapabilities> {
 *      /// wrapper to execute the algorithm on requested provided device
 *      /// @return A dataype matching the tester requirements to verify the results
 *      ResultsType do_task_on_device(DeviceType&) const;
 * };
 * @endcode
 * Instantiate your algorithm tests by constructing suitable @class AlgorithmTestTraits classes
 * and then instantiate them with the INSTANTIATE_TYPED_TEST_SUITE_P macro
 *
 *  e.g.
 * @code
 * typedef ::testing::Types<MyAlgoTraits1<unit8_t>, MyAlgoTraits1<int>, MyAlgoTraits2<unit8_t>> MyTypes;
 * INSTANTIATE_TYPED_TEST_SUITE_P(MyAlgo, RfimTester, MyTypes);
 * @endcode
 *  n.b. the INSTANTIATE_TYPED_TEST_SUITE_P must be in the same namespace as this class
 *
 */

/**
 * @brief Specialise to set non-standard pass/fail criteria
 */
template<class TestTypeTraits, class Scenario>
struct EvaluateRfimMetrics
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 15);
        EXPECT_LT(m.false_negatives_percentage(), 25);
    }
};

template<typename NumericalT, typename ArchitectureTag, typename ArchitectureCapability=ArchitectureTag>
struct RfimTesterTraits : public utils::test::AlgorithmTesterTraits<ArchitectureTag, ArchitectureCapability> {
    typedef utils::test::AlgorithmTesterTraits<ArchitectureTag, ArchitectureCapability> BaseT;
    typedef typename BaseT::DeviceType DeviceType;
    typedef typename data::TimeFrequency<Cpu, NumericalT> DataType;
    typedef typename data::RfimFlaggedData<DataType> FlaggedDataType;
    typedef typename std::shared_ptr<FlaggedDataType> ResultType;
    typedef rfim::FlagPolicy<DataType> Policy;

    static ResultType apply_algorithm(DeviceType&, DataType);
};


template <typename TestTraits>
class RfimTester : public cheetah::utils::test::AlgorithmTester<TestTraits>
{
    protected:
        TestTraits _traits;

        void SetUp() override;
        void TearDown() override;

    public:
        RfimTester();
        ~RfimTester();

        static void verify_equal(typename TestTraits::DataType const& , typename TestTraits::DataType const&);
};

TYPED_TEST_SUITE_P(RfimTester);

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/rfim/test_utils/detail/RfimTester.cpp"


#endif // SKA_CHEETAH_MODULES_RFIM_TEST_RFIMTESTER_H
