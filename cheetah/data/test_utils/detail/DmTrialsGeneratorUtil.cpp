#include "cheetah/data/test_utils/DmTrialsGeneratorUtil.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template <typename DmTrialsType>
DmTrialsGeneratorUtil<DmTrialsType>::DmTrialsGeneratorUtil()
    : _epoch(TimePoint(utils::julian_day(40587.0)))
{

}

template <typename DmTrialsType>
DmTrialsGeneratorUtil<DmTrialsType>::~DmTrialsGeneratorUtil()
{

}

template <typename DmTrialsType>
std::shared_ptr<DmTrialsType> DmTrialsGeneratorUtil<DmTrialsType>::generate(
    typename DmTrialsType::TimeType sampling_interval,
    std::size_t fundamental_sample_count,
    std::size_t ndms)
{
    auto metadata = DmTrialsMetadata::make_shared(sampling_interval,fundamental_sample_count);
    for (std::size_t dm_idx=0; dm_idx<ndms; ++dm_idx)
    {
        metadata->emplace_back(DmTrialsMetadata::DmType(dm_idx * data::parsecs_per_cube_cm), 1);
    }
    auto trials = DmTrialsType::make_shared(metadata,_epoch);
    _epoch += std::chrono::duration<double>(trials->duration().value());
    return trials;
}

template <typename DmTrialsType>
typename DmTrialsGeneratorUtil<DmTrialsType>::TimePoint DmTrialsGeneratorUtil<DmTrialsType>::epoch() const
{
    return _epoch;
}

template <typename DmTrialsType>
void DmTrialsGeneratorUtil<DmTrialsType>::epoch(TimePoint t)
{
    _epoch = t;
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
