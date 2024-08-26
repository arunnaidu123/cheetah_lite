#include "cheetah/data/detail/DmTimeSliceIterator.h"
#include "cheetah/data/detail/DmTimeSlice.h"
#include "cheetah/data/detail/DmTimeDm.h"

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

template <typename DmTimeSliceType>
DmTimeSliceIterator<DmTimeSliceType>::DmTimeSliceIterator(std::size_t current_dm_idx,
    std::shared_ptr<const DmTimeSliceType> parent)
    : _current_dm_idx(current_dm_idx)
    , _parent(parent)
{
}

template <typename DmTimeSliceType>
DmTimeSliceIterator<DmTimeSliceType>::~DmTimeSliceIterator()
{
}

template <typename DmTimeSliceType>
typename DmTimeSliceIterator<DmTimeSliceType>::DmTimeDmType DmTimeSliceIterator<DmTimeSliceType>::operator*() const
{
    return DmTimeDmType(_current_dm_idx, _parent);
}

template <typename DmTimeSliceType>
DmTimeSliceIterator<DmTimeSliceType>& DmTimeSliceIterator<DmTimeSliceType>::operator++()
{
    ++_current_dm_idx;
    return *this;
}

template <typename DmTimeSliceType>
bool DmTimeSliceIterator<DmTimeSliceType>::operator!=(DmTimeSliceIterator const& other)
{
    return  _current_dm_idx != other._current_dm_idx;
}

template <typename DmTimeSliceType>
bool DmTimeSliceIterator<DmTimeSliceType>::operator==(DmTimeSliceIterator const& other)
{
    return  _current_dm_idx == other._current_dm_idx;
}

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska
