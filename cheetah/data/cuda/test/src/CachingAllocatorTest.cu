#include "cheetah/data/test_utils/CachingAllocatorTest.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

typedef ::testing::Types<
    CachingAllocatorTestTraits<cheetah::Cuda,float>,
    CachingAllocatorTestTraits<cheetah::Cuda,double>,
    CachingAllocatorTestTraits<cheetah::Cuda,char>
    > CudaCachingAllocatorTestTypes;
TYPED_TEST_SUITE(CachingAllocatorTest, CudaCachingAllocatorTestTypes);

TYPED_TEST(CachingAllocatorTest, test_alloc_dealloc)
{
    AllocateDeallocateTest<TypeParam>::test();
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
