/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#include "cheetah/modules/rfim/iqrmcpu/test/IqrmCpuTest.h"
#include "cheetah/modules/rfim/iqrmcpu/Rfim.h"
#include "cheetah/modules/rfim/test_utils/RfimTester.h"
#include <utility>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {

template<typename NumericalRepT>
struct IqrmCpuTestRfimTraits : public rfim::DefaultRfimTraits<data::TimeFrequency<Cpu, NumericalRepT>>
{
    typedef NumericalRepT NumericalRep;
    typedef rfim::DefaultRfimTraits<data::TimeFrequency<Cpu, NumericalRep>> BaseT;
    typedef data::TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    using typename BaseT::Policy;
    using typename BaseT::BandPassHandler;
};

template<typename NumericalRepT>
class IqrmCpuTraits : public RfimTesterTraits<NumericalRepT, typename rfim::iqrmcpu::Rfim<IqrmCpuTestRfimTraits<NumericalRepT>>::Architecture>
{
        typedef RfimTesterTraits<NumericalRepT, typename rfim::iqrmcpu::Rfim<IqrmCpuTestRfimTraits<NumericalRepT>>::Architecture> BaseT;

    public:
        typedef typename BaseT::ResultType ResultType;
        typedef typename BaseT::DataType DataType;

    public:
        // return the iqrm specific configuration
        static rfim::iqrmcpu::Config& config()
        {
            static rfim::iqrmcpu::Config config;
            return config;
        }

        // return the api for the iqrm algorithm
        static rfim::iqrmcpu::Rfim<IqrmCpuTestRfimTraits<NumericalRepT>>& algo()
        {
            static rfim::iqrmcpu::Rfim<IqrmCpuTestRfimTraits<NumericalRepT>> algo_api(config());
            return algo_api;
        }

        template<typename DeviceT>
        ResultType apply_algorithm(DeviceT&& device, DataType& data)
        {
            return algo()(std::forward<DeviceT>(device), data.shared_from_this());
        }

};

// non standard pass/fail criteria
template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<4, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 25);
        EXPECT_LT(m.false_negatives_percentage(), 24);
    }
};

template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<5, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 80);
        EXPECT_LT(m.false_negatives_percentage(), 25);
    }
};

template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<6, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 75);
        EXPECT_LT(m.false_negatives_percentage(), 30);
    }
};

template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<7, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 15);
        EXPECT_LT(m.false_negatives_percentage(), 33);
    }
};

template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<8, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 14);
        EXPECT_LT(m.false_negatives_percentage(), 34);
    }
};

template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<9, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 14);
        EXPECT_LT(m.false_negatives_percentage(), 35);
    }
};

template<typename NumericalRep>
struct EvaluateRfimMetrics<IqrmCpuTraits<NumericalRep>, cheetah::generators::RfiScenario<10, NumericalRep>>
{
    static
    void evaluate(rfim::Metrics const& m) {
        EXPECT_LT(m.false_positives_percentage(), 5);
        EXPECT_LT(m.false_negatives_percentage(), 70);
    }
};

// -- generate the tests
typedef ::testing::Types<IqrmCpuTraits<uint8_t>, IqrmCpuTraits<float>> IqrmCpuTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(IqrmCpu, RfimTester, IqrmCpuTraitsTypes);

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
