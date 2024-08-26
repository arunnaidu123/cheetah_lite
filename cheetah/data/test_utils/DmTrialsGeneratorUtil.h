#ifndef SKA_CHEETAH_DATA_TEST_UTILS_DMTRIALSGENERATORUTIL_H
#define SKA_CHEETAH_DATA_TEST_UTILS_DMTRIALSGENERATORUTIL_H

#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/Units.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {


template <typename DmTrialsType>
class DmTrialsGeneratorUtil
{

    public:
        typedef typename utils::ModifiedJulianClock::time_point TimePoint;

    public:
        DmTrialsGeneratorUtil();
        ~DmTrialsGeneratorUtil();

        std::shared_ptr<DmTrialsType> generate(
            typename DmTrialsType::TimeType sampling_interval_seconds,
            std::size_t fundamental_sample_count,
            std::size_t ndms);

        TimePoint epoch() const;
        void epoch(TimePoint t);

    private:
        TimePoint _epoch;

};

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/test_utils/detail/DmTrialsGeneratorUtil.cpp"

#endif //SKA_CHEETAH_DATA_TEST_UTILS_DMTRIALSGENERATORUTIL_H
