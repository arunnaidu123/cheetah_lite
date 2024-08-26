#include "cheetah/data/SeriesSlice.h"
#include <iterator>

namespace ska {
namespace cheetah {
namespace data {

template <typename SeriesType>
SeriesSlice<SeriesType>::SeriesSlice(Iterator start_it, Iterator end_it)
    : _start_it(std::move(start_it))
    , _end_it(std::move(end_it))
{
}

template <typename SeriesType>
SeriesSlice<SeriesType>::~SeriesSlice()
{
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::Iterator SeriesSlice<SeriesType>::begin()
{
    return _start_it;
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::ConstIterator SeriesSlice<SeriesType>::begin() const
{
    return _start_it;
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::ConstIterator SeriesSlice<SeriesType>::cbegin() const
{
    return _start_it;
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::Iterator SeriesSlice<SeriesType>::end()
{
    return _end_it;
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::ConstIterator SeriesSlice<SeriesType>::end() const
{
    return _end_it;
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::ConstIterator SeriesSlice<SeriesType>::cend() const
{
    return _end_it;
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::Reference SeriesSlice<SeriesType>::operator[](std::size_t n)
{
    return *(_start_it + n);
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::ConstReference SeriesSlice<SeriesType>::operator[](std::size_t n) const
{
    return *(_start_it + n);
}

template <typename SeriesType>
std::size_t SeriesSlice<SeriesType>::size() const
{
    return std::distance(_start_it,_end_it);
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::Pointer SeriesSlice<SeriesType>::data()
{
    return &(*(_start_it));
}

template <typename SeriesType>
typename SeriesSlice<SeriesType>::ConstPointer SeriesSlice<SeriesType>::data() const
{
    return &(*(_start_it));
}

} // namespace data
} // namespace cheetah
} // namespace ska
