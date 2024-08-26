#include "cheetah/modules/rfim/sum_threshold/test/SumThresholdTest.h"
#include "cheetah/modules/rfim/sum_threshold/Rfim.h"
#include "cheetah/modules/rfim/test_utils/RfimTester.h"
#include <memory>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace sum_threshold {
namespace test {

template<typename NumericalRepT>
struct SumThresholdRfimTraits : public rfim::DefaultRfimTraits<data::TimeFrequency<Cpu, NumericalRepT>>
{
    typedef NumericalRepT NumericalRep;
    typedef rfim::DefaultRfimTraits<data::TimeFrequency<Cpu, NumericalRep>> BaseT;
    typedef data::TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef rfim::ampp::Spectrum<NumericalRep> BandPassType;
    using typename BaseT::Policy;
    using typename BaseT::BandPassHandler;
};

template<typename NumericalRep>
struct SumThresholdTraits : public rfim::test::RfimTesterTraits<NumericalRep, cheetah::Cpu> {
    public:
        typedef rfim::test::RfimTesterTraits<NumericalRep, cheetah::Cpu> BaseT;
        typedef typename BaseT::ResultType ResultType;
        typedef typename BaseT::DataType DataType;

        SumThresholdTraits();

        template<typename DeviceType>
        ResultType apply_algorithm(DeviceType&&, DataType& data);

        rfim::sum_threshold::Rfim<SumThresholdRfimTraits<NumericalRep>>& algo();

    private:
        rfim::sum_threshold::Config _config;
        rfim::sum_threshold::Rfim<SumThresholdRfimTraits<NumericalRep>> _algo;
        bool _initialised;
};

template<typename NumericalRep>
SumThresholdTraits<NumericalRep>::SumThresholdTraits()
    : _algo(_config)
{
}

template<typename NumericalRep>
template<typename DeviceType>
typename SumThresholdTraits<NumericalRep>::ResultType SumThresholdTraits<NumericalRep>::apply_algorithm(DeviceType&& d, DataType& data)
{
    return algo()(std::forward<DeviceType>(d), data.shared_from_this());
}

template<typename NumericalRep>
rfim::sum_threshold::Rfim<SumThresholdRfimTraits<NumericalRep>>& SumThresholdTraits<NumericalRep>::algo()
{
    //static rfim::sum_threshold::Rfim<SumThresholdRfimTraits<NumericalRep>> algo_api(_config);
    return _algo;
}

SumThresholdTest::SumThresholdTest()
    : ::testing::Test()
{
}

SumThresholdTest::~SumThresholdTest()
{
}

void SumThresholdTest::SetUp()
{
}

void SumThresholdTest::TearDown()
{
}

/*
TEST_F(SumThresholdTest, test_something)
{
}
*/


} // namespace test
} // namespace sum_threshold
namespace test {
    typedef ::testing::Types<rfim::sum_threshold::test::SumThresholdTraits<uint8_t>> SumThresholdTraitsTypes;
    INSTANTIATE_TYPED_TEST_SUITE_P(SumThreshold, RfimTester, SumThresholdTraitsTypes);
} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
