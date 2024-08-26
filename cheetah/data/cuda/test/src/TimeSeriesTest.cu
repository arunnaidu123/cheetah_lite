#include "cheetah/data/test_utils/TimeSeriesTester.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

typedef ::testing::Types<
    TimeSeriesTesterTraits<TimeSeries<cheetah::Cuda, float>>,
    TimeSeriesTesterTraits<TimeSeries<cheetah::Cuda, double>>,
    TimeSeriesTesterTraits<TimeSeries<cheetah::Cuda, char>>
    > TimeSeriesTesterTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TimeSeries, TimeSeriesTester, TimeSeriesTesterTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
