#include "cheetah/utils/Architectures.h"
#include "cheetah/data/test_utils/DmTrialsTester.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template<typename ValueType>
class CudaDmTrialsTestTraits : public DmTrialsTesterTraits<data::DmTrials<cheetah::Cuda, ValueType>>
{
        typedef DmTrialsTesterTraits<data::DmTrials<cheetah::Cuda, ValueType>> BaseT;
        typedef typename BaseT::Allocator Allocator;

    public:
        Allocator allocator(panda::PoolResource<cheetah::Cuda>&) override { return Allocator(); }
};

typedef ::testing::Types<CudaDmTrialsTestTraits<double>, CudaDmTrialsTestTraits<float>, CudaDmTrialsTestTraits<uint8_t>> CudaDmTrialsTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(BasicCudaDmTrials, DmTrialsTester, CudaDmTrialsTraitsTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
