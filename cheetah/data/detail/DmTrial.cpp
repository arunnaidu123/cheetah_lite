#include "cheetah/data/DmTrial.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename SeriesType>
DmTrial<SeriesType>::DmTrial(DmTrialsMetadata::Metadata const& metadata, Iterator start_it, Iterator end_it)
    : SeriesSlice<SeriesType>(std::move(start_it),std::move(end_it))
    , _metadata(metadata)
{
}

template <typename SeriesType>
DmTrial<SeriesType>::~DmTrial()
{
}

template <typename SeriesType>
typename DmTrial<SeriesType>::DmType DmTrial<SeriesType>::dm() const
{
    return _metadata.dm();
}

template <typename SeriesType>
typename DmTrial<SeriesType>::TimeType DmTrial<SeriesType>::sampling_interval() const
{
    return _metadata.sampling_interval();
}

} // namespace data
} // namespace cheetah
} // namespace ska
