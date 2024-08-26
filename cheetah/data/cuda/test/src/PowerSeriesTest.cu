#include "cheetah/data/test_utils/PowerSeriesTest.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

typedef ::testing::Types<
    PowerSeriesTestTraits<cheetah::Cuda,float>,
    PowerSeriesTestTraits<cheetah::Cuda,double>,
    PowerSeriesTestTraits<cheetah::Cuda,char>
    > PowerSeriesTestTypes;
TYPED_TEST_SUITE(PowerSeriesTest, PowerSeriesTestTypes);

TYPED_TEST(PowerSeriesTest, test_bins)
{
    SampleCountTest<TypeParam>::test(1000L);
    SampleCountTest<TypeParam>::test(1<<23);
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
