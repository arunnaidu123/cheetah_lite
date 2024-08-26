#include "cheetah/data/detail/DmTimeSlice.h"
#include "cheetah/data/detail/DmTimeSliceIterator.h"

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

template <typename DmTimeType>
DmTimeSlice<DmTimeType>::DmTimeSlice(std::size_t start_dm_idx, std::size_t number_dms_per_slice,
    ParentType parent)
    : _parent(parent)
    , _start_dm_idx(start_dm_idx)
    , _number_dms_per_slice(number_dms_per_slice)
{
}

template <typename DmTimeType>
DmTimeSlice<DmTimeType>::~DmTimeSlice()
{
}

template <typename DmTimeType>
typename DmTimeSlice<DmTimeType>::Iterator DmTimeSlice<DmTimeType>::begin()
{
    return Iterator(_start_dm_idx, this->shared_from_this());
}

template <typename DmTimeType>
typename DmTimeSlice<DmTimeType>::ConstIterator DmTimeSlice<DmTimeType>::cbegin() const
{
    return ConstIterator(_start_dm_idx, this->shared_from_this());
}

template <typename DmTimeType>
typename DmTimeSlice<DmTimeType>::Iterator DmTimeSlice<DmTimeType>::end()
{
    // blocks is guaranteed to never be empty so front() will always return
    std::size_t end_idx = std::min(blocks().front()->size(),_start_dm_idx+_number_dms_per_slice);
    return Iterator(end_idx, this->shared_from_this());
}

template <typename DmTimeType>
typename DmTimeSlice<DmTimeType>::ConstIterator DmTimeSlice<DmTimeType>::cend() const
{
    // blocks is guaranteed to never be empty so front() will always return
    std::size_t end_idx = std::min(blocks().front()->size(),_start_dm_idx+_number_dms_per_slice);
    return ConstIterator(end_idx, this->shared_from_this());
}

template <typename DmTimeType>
typename DmTimeSlice<DmTimeType>::ContainerType const& DmTimeSlice<DmTimeType>::blocks() const
{
    return _parent->blocks();
}

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska
