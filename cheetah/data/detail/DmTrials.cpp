#include "cheetah/data/DmTrials.h"
#include "panda/Error.h"
#include "panda/Log.h"
#include <chrono>

namespace ska {
namespace cheetah {
namespace data {


template <typename Arch, typename T, typename Alloc>
DmTrials<Arch,T,Alloc>::DmTrials(std::shared_ptr<DmTrialsMetadata> metadata, Mjd start, Alloc const& allocator)
    : _metadata(std::move(metadata))
    , _data((_metadata?_metadata->total_data_size():0), allocator)
    , _start_time(std::move(start))
{
    if (!_metadata || _metadata->size() == 0)
        throw panda::Error("Empty DmTrialsMetadata passed to DmTrials constructor.");
    _trials.reserve(_metadata->size());
    auto it = _data.begin();
    for (auto const& trial: *_metadata)
    {
        std::size_t const size = trial.size();
        _trials.emplace_back(trial,it,it+size);
        it+=size;
    }
}

template <typename Arch, typename T, typename Alloc>
template<typename OtherArch, typename OtherT, typename OtherAlloc>
DmTrials<Arch,T,Alloc>::DmTrials(DmTrials<OtherArch, OtherT, OtherAlloc> const& copy, Alloc const& allocator)
    : _metadata(std::make_shared<DmTrialsMetadata>(copy.metadata()))
    , _data(copy._data, allocator)
    , _start_time(copy.start_time())
    , _duration(copy.duration())
{
    _trials.reserve(_metadata->size());
    auto it = _data.begin();
    for (auto const& trial: *_metadata)
    {
        std::size_t const size = trial.size();
        _trials.emplace_back(trial,it,it+size);
        it+=size;
    }
}

template <typename Arch, typename T, typename Alloc>
DmTrials<Arch,T,Alloc>::~DmTrials()
{
}

template <typename Arch, typename T, typename Alloc>
DmTrialsMetadata const& DmTrials<Arch, T, Alloc>::metadata() const
{
    return *_metadata;
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::Mjd const& DmTrials<Arch,T,Alloc>::start_time() const
{
    return _start_time;
}

template <typename Arch, typename T, typename Alloc>
void DmTrials<Arch,T,Alloc>::start_time(Mjd start)
{
    _start_time = start;
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::DmTrialType& DmTrials<Arch,T,Alloc>::operator[](std::size_t n)
{
    return _trials[n];
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::DmTrialType const& DmTrials<Arch,T,Alloc>::operator[](std::size_t n) const
{
    return _trials[n];
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::Iterator DmTrials<Arch,T,Alloc>::begin()
{
    return _trials.begin();
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::ConstIterator DmTrials<Arch,T,Alloc>::begin() const
{
    return _trials.cbegin();
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::ConstIterator DmTrials<Arch,T,Alloc>::cbegin() const
{
    return _trials.cbegin();
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::Iterator DmTrials<Arch,T,Alloc>::end()
{
    return _trials.end();
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::ConstIterator DmTrials<Arch,T,Alloc>::end() const
{
    return _trials.end();
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::ConstIterator DmTrials<Arch,T,Alloc>::cend() const
{
    return _trials.cend();
}

template <typename Arch, typename T, typename Alloc>
std::size_t DmTrials<Arch,T,Alloc>::size() const
{
    return _trials.size();
}

template <typename Arch, typename T, typename Alloc>
bool DmTrials<Arch,T,Alloc>::is_compatible(DmTrials const& other) const
{
    return *_metadata == *(other._metadata);
}

template <typename Arch, typename T, typename Alloc>
bool DmTrials<Arch,T,Alloc>::is_contiguous(DmTrials const& next) const
{

    TimeType diff(pss::astrotypes::units::duration_cast<TimeType>(next._start_time - (_start_time + std::chrono::duration<double>(this->duration().value()))));
    return  diff < _metadata->fundamental_sampling_interval();
}

template <typename Arch, typename T, typename Alloc>
typename DmTrials<Arch,T,Alloc>::TimeType DmTrials<Arch,T,Alloc>::duration() const
{
    return _metadata->duration();
}

template <typename Arch, typename T, typename Alloc>
void DmTrials<Arch,T,Alloc>::swap(DmTrials& b)
{
    std::swap(_metadata, b._metadata);
    _data.swap(b._data);
    _trials.swap(b._trials);
    std::swap(_start_time, b._start_time);
    std::swap(_duration, b._duration);
}

} // namespace data
} // namespace cheetah
} // namespace ska

