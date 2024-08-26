#include "cheetah/data/detail/DmTimeDm.h"
#include "cheetah/data/detail/DmTimeSlice.h"
#include "cheetah/data/DmTime.h"

#include "panda/Copy.h"
#include "panda/Log.h"

#include <typeinfo>
#include <algorithm>

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

template <typename DmTimeType>
DmTimeDm<DmTimeType>::DmTimeDm(std::size_t const dm_idx, std::shared_ptr<const SliceType> parent)
    : _dm_idx(dm_idx)
    , _parent(std::move(parent))
{
}

template <typename DmTimeType>
DmTimeDm<DmTimeType>::~DmTimeDm()
{
}

template <typename DmTimeType>
std::size_t DmTimeDm<DmTimeType>::number_of_samples() const
{
    std::size_t sample_count = 0;
    for (auto const& block: _parent->blocks())
        sample_count += block->operator[](_dm_idx).size();
    return sample_count;
}

template <typename DmTimeType>
data::DedispersionMeasureType<float> DmTimeDm<DmTimeType>::dm() const
{
    if ( _parent->blocks().empty())
    {
        throw std::runtime_error("Requested DM from empty DmTime object.");
    }
    return _parent->blocks().front()->operator[](_dm_idx).dm();
}

template <typename DmTimeType>
typename DmTimeDm<DmTimeType>::TimeType DmTimeDm<DmTimeType>::sampling_interval() const
{
    if ( _parent->blocks().empty())
    {
        throw std::runtime_error("Requested sampling interval from empty DmTime object.");
    }
    return _parent->blocks().front()->operator[](_dm_idx).sampling_interval();
}

template <typename DmTimeType>
template <typename TimeSeriesType>
typename TimeSeriesType::Iterator DmTimeDm<DmTimeType>::copy_to(TimeSeriesType& timeseries) const
{
    if ( _parent->blocks().empty())
    {
        throw std::runtime_error("Requested copy from empty DmTime object.");
    }
    timeseries.sampling_interval(_parent->blocks().front()->operator[](_dm_idx).sampling_interval());
    std::size_t count = timeseries.size();
    std::size_t copied = 0;
    typename TimeSeriesType::Iterator it = timeseries.end();
    for (auto const& block: _parent->blocks())
    {
        auto dmtrial = block->operator[](_dm_idx);
        std::size_t copy_count = std::min(dmtrial.size(),count-copied);
        PANDA_LOG_DEBUG << "Copying "<<copy_count<<" samples from DmTime block to timeseries";
        auto device_it = timeseries.begin()+copied;
        it = panda::copy(dmtrial.begin(), dmtrial.begin()+copy_count, device_it);
        copied = std::distance(timeseries.begin(),it);
        if (copied == count)
            break;
    }
    return it;
}

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska
