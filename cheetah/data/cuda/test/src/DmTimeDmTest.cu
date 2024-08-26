#include "cheetah/data/test_utils/DmTimeDmTest.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

TEST_F(DmTimeDmTest, test_cuda_copy_same_lengths)
{
    CopyTest<cheetah::Cuda>::test(5,333,5*333);
}

TEST_F(DmTimeDmTest, test_cuda_copy_longer_timeseries)
{
    CopyTest<cheetah::Cuda>::test(5,222,222*5+123);
}

TEST_F(DmTimeDmTest, test_cuda_copy_longer_dm_trial)
{
    CopyTest<cheetah::Cuda>::test(5,111,111*3+45);
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska