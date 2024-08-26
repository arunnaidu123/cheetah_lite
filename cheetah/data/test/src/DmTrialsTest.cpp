#include "cheetah/utils/Architectures.h"
#include "cheetah/data/test_utils/DmTrialsTester.h"
#include <panda/test/TestArch.h>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template<typename ValueType>
class CpuDmTrialsTestTraits : public DmTrialsTesterTraits<data::DmTrials<panda::Cpu, ValueType>>
{
        typedef DmTrialsTesterTraits<data::DmTrials<panda::Cpu, ValueType>> BaseT;
        typedef typename BaseT::Allocator Allocator;

    public:
        Allocator allocator(panda::PoolResource<panda::Cpu>&) override { return Allocator(); }
};

typedef ::testing::Types<CpuDmTrialsTestTraits<double>, CpuDmTrialsTestTraits<float>, CpuDmTrialsTestTraits<uint8_t>> CpuDmTrialsTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(BasicCpuDmTrials, DmTrialsTester, CpuDmTrialsTraitsTypes);

template<typename ValueType>
class TestArchDmTrialsTestTraits : public DmTrialsTesterTraits<data::DmTrials<panda::test::TestArch, ValueType>>
{
        typedef DmTrialsTesterTraits<data::DmTrials<panda::test::TestArch, ValueType>> BaseT;
        typedef typename BaseT::Allocator Allocator;

    public:
        Allocator allocator(panda::PoolResource<panda::test::TestArch>& device) override { return Allocator(device); }
};

typedef ::testing::Types<TestArchDmTrialsTestTraits<double>, TestArchDmTrialsTestTraits<float>, TestArchDmTrialsTestTraits<uint8_t>> TestArchDmTrialsTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TestArchDmTrials, DmTrialsTester, TestArchDmTrialsTraitsTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
