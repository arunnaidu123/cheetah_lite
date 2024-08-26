#include "cheetah/modules/rfim/policy/LastUnflagged.h"
#include <algorithm>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

template<typename TimeFrequencyType>
LastUnflagged<TimeFrequencyType>::LastUnflagged()
    : _replacement_sample(8)
    , _sample_index(_replacement_sample.size(), 0)
    , _read_index(_sample_index)
{
}

template<typename TimeFrequencyType>
LastUnflagged<TimeFrequencyType>::~LastUnflagged()
{
}

template<typename TimeFrequencyType>
typename LastUnflagged<TimeFrequencyType>::Adapter LastUnflagged<TimeFrequencyType>::adapter(TimeFrequencyType& data)
{
    if(_replacement_sample[_read_index].size() != data.template dimension<data::Frequency>())
    {
        _replacement_sample[_read_index].resize(data.template dimension<data::Frequency>());
    }
    return Adapter(*this, data);
}

template<typename TimeFrequencyType>
void LastUnflagged<TimeFrequencyType>::set_good_sample(Spectrum const& sample)
{
    unsigned index = ++_sample_index;
    std::copy(sample.begin(), sample.end(), _replacement_sample[index].begin());
    if(_read_index < index) { _read_index = index; }
}

template<typename TimeFrequencyType>
void LastUnflagged<TimeFrequencyType>::set_good_sample(NumericalRep const& sample_start, std::size_t size)
{
    unsigned index = ++_sample_index;
    auto& replacement = _replacement_sample[index];
    replacement.resize(size);
    std::copy(&sample_start, &sample_start + size, replacement.begin());
    if(_read_index < index) { _read_index = index; }
}

template<typename TimeFrequencyType>
typename LastUnflagged<TimeFrequencyType>::SampleType& LastUnflagged<TimeFrequencyType>::last_good_spectrum()
{
    return _replacement_sample[_read_index];
}

template<typename TimeFrequencyType>
LastUnflaggedAdapter<TimeFrequencyType>::LastUnflaggedAdapter(LastUnflagged<TimeFrequencyType>& parent, TimeFrequencyType& data)
    : _parent(parent)
    , _data(&data)
    , _replacement_sample(parent.last_good_spectrum())
    , _last_good_spectrum(&*(_replacement_sample.begin()))
{
}

template<typename TimeFrequencyType>
LastUnflaggedAdapter<TimeFrequencyType>::~LastUnflaggedAdapter()
{
    _parent.set_good_sample(*_last_good_spectrum, _data->template dimension<data::Frequency>());
}

template<typename TimeFrequencyType>
void LastUnflaggedAdapter<TimeFrequencyType>::data()
{
}

template<typename TimeFrequencyType>
inline void LastUnflaggedAdapter<TimeFrequencyType>::mark_bad(NumericalRep const& data, data::DimensionIndex<data::Frequency> channel_number)
{
    const_cast<NumericalRep&>(data) = *(_last_good_spectrum + channel_number);
}

template<typename TimeFrequencyType>
inline void LastUnflaggedAdapter<TimeFrequencyType>::mark_good(NumericalRep const& data, data::DimensionIndex<data::Frequency> channel_number)
{
    _replacement_sample[channel_number] = data;
}

template<typename TimeFrequencyType>
template<typename SliceT>
typename std::enable_if<pss::astrotypes::is_slice<SliceT>::value, void>::type
LastUnflaggedAdapter<TimeFrequencyType>::mark_good(SliceT const& slice)
{
    std::copy(slice.begin(), slice.end(), _replacement_sample.begin() + slice.template span<data::Frequency>().start());
}

template<typename TimeFrequencyType>
template<typename SliceType>
typename std::enable_if<pss::astrotypes::is_slice<SliceType>::value && std::remove_reference<SliceType>::type::rank != 1, void>::type
LastUnflaggedAdapter<TimeFrequencyType>::mark_bad(SliceType const& slice)
{
    auto& writable_slice = pss::astrotypes::multiarray::cast_to_non_const_slice(const_cast<SliceType&>(slice));
    const NumericalRep* ptr = _last_good_spectrum + static_cast<std::size_t>(writable_slice.template span<data::Frequency>().start());
    const NumericalRep* end_ptr = _last_good_spectrum + static_cast<std::size_t>(writable_slice.template dimension<data::Frequency>());
    for(data::DimensionIndex<data::Time> spectrum_num(0); spectrum_num < slice.template dimension<data::Time>(); ++spectrum_num)
    {
        std::copy(ptr, end_ptr, &*writable_slice[spectrum_num].begin());
    }
}

template<typename TimeFrequencyType>
template<typename SliceType>
typename std::enable_if<pss::astrotypes::is_slice<SliceType>::value && std::remove_reference<SliceType>::type::rank == 1, void>::type
LastUnflaggedAdapter<TimeFrequencyType>::mark_bad(SliceType const& slice)
{
    auto& writable_slice = pss::astrotypes::multiarray::cast_to_non_const_slice(const_cast<SliceType&>(slice));
    const NumericalRep* ptr = _last_good_spectrum + static_cast<std::size_t>(writable_slice.template span<data::Frequency>().start());
    const NumericalRep* end_ptr = _last_good_spectrum + static_cast<std::size_t>(writable_slice.template dimension<data::Frequency>());
    std::copy(ptr, end_ptr, &*writable_slice.begin());
}

template<typename TimeFrequencyType>
void LastUnflaggedAdapter<TimeFrequencyType>::mark_bad(ConstChannel const& channel)
{
    this->mark_bad( pss::astrotypes::multiarray::cast_to_non_const_slice(const_cast<ConstChannel&>(channel)));
}

template<typename TimeFrequencyType>
void LastUnflaggedAdapter<TimeFrequencyType>::mark_bad(Channel const& channel)
{
    Channel& writable_channel = const_cast<Channel&>(channel);
    auto const& channel_value = _last_good_spectrum[channel.template span<data::Frequency>().start()];
    std::fill(writable_channel.begin(), writable_channel.end(), channel_value);
}

template<typename TimeFrequencyType>
void LastUnflaggedAdapter<TimeFrequencyType>::mark_bad(data::DimensionIndex<data::Frequency> channel_number)
{
    auto const max = _data->template dimension<data::Time>();
    for(data::DimensionIndex<data::Time> spectrum_num(0); spectrum_num < max; ++spectrum_num)
    {
        // replace each spectrum in the channel with the corresponding value of the last good spectrum
        _data[spectrum_num] = *(_last_good_spectrum + static_cast<std::size_t>(channel_number));
    }
}


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
