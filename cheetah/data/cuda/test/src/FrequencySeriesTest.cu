#include "cheetah/data/test_utils/FrequencySeriesTester.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

//The commented test inputs can be uncommented once the copy between Cuda to non-cuda or cpu
//has been fixed. Current code as it stands throws compilation errors.
typedef ::testing::Types<
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, float>, FrequencySeries<cheetah::Cuda, float>>,
    //FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, float>, FrequencySeries<cheetah::Cuda, double>>,
    //FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, double>, FrequencySeries<cheetah::Cuda, float>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, double>, FrequencySeries<cheetah::Cuda, double>>,
    //FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, char>, FrequencySeries<cheetah::Cuda, float>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, thrust::complex<float>>, FrequencySeries<cheetah::Cuda, thrust::complex<float>>>,
    //FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, thrust::complex<float>>, FrequencySeries<cheetah::Cuda, thrust::complex<double>>>,
    //FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, thrust::complex<double>>, FrequencySeries<cheetah::Cuda, thrust::complex<float>>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cuda, thrust::complex<double>>, FrequencySeries<cheetah::Cuda, thrust::complex<double>>>
    > FrequencySeriesTesterTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(FrequencySeries, FrequencySeriesTester, FrequencySeriesTesterTypes);



} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
