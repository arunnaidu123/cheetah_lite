#include "cheetah/data/detail/DmTimeIterator.h"

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

template <typename DmTimeType>
DmTimeIterator<DmTimeType>::DmTimeIterator(std::size_t current_dm_idx,
    std::size_t number_dms_per_slice, std::shared_ptr<DmTimeType> parent)
    : _current_dm_idx(current_dm_idx)
    , _number_dms_per_slice(number_dms_per_slice)
    , _parent(parent)
{
}

template <typename DmTimeType>
DmTimeIterator<DmTimeType>::~DmTimeIterator()
{
}

template <typename DmTimeType>
DmTimeIterator<DmTimeType>& DmTimeIterator<DmTimeType>::operator++()
{
    _current_dm_idx+=_number_dms_per_slice;
    return *this;
}

template <typename DmTimeType>
std::shared_ptr<typename DmTimeIterator<DmTimeType>::SliceType> DmTimeIterator<DmTimeType>::operator*() const
{
    return SliceType::make_shared(_current_dm_idx, _number_dms_per_slice, _parent);
}

template <typename DmTimeType>
bool DmTimeIterator<DmTimeType>::operator!=(DmTimeIterator const& other) const
{
    return _current_dm_idx != other._current_dm_idx;
}

template <typename DmTimeType>
bool DmTimeIterator<DmTimeType>::operator==(DmTimeIterator const& other) const
{
    return _current_dm_idx == other._current_dm_idx;
}

template <typename DmTimeType>
bool DmTimeIterator<DmTimeType>::operator<(DmTimeIterator const& other) const
{
    return _current_dm_idx < other._current_dm_idx;
}

template <typename DmTimeType>
bool DmTimeIterator<DmTimeType>::operator>(DmTimeIterator const& other) const
{
    return _current_dm_idx > other._current_dm_idx;
}

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska
