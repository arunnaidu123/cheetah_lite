#include <type_traits>
#include <algorithm>


namespace ska {
namespace cheetah {
namespace data {


template<class Arch>
TimeFrequencyFlags<Arch>::TimeFrequencyFlags(){}

template<class Arch>
TimeFrequencyFlags<Arch>::TimeFrequencyFlags(pss::astrotypes::DimensionSize<Time> number_of_spectra, pss::astrotypes::DimensionSize<Frequency> number_of_channels)
    : BaseT(number_of_channels, number_of_spectra)
{
}

template<class Arch>
TimeFrequencyFlags<Arch>::~TimeFrequencyFlags(){}

template<class Arch>
std::size_t TimeFrequencyFlags<Arch>::number_of_channels() const {return this->template dimension<Frequency>();}

template<class Arch>
std::size_t TimeFrequencyFlags<Arch>::number_of_spectra() const {return this->template dimension<Time>();}

/*
template<class Arch>
typename TimeFrequencyFlags<Arch>::ChannelRange TimeFrequencyFlags<Arch>::time_sample(std::size_t sample_number) 
{
    return this->slice(0, this->fast_axis_length(), sample_number, sample_number +1);
}

template<class Arch>
typename TimeFrequencyFlags<Arch>::ConstChannelRange TimeFrequencyFlags<Arch>::time_sample(std::size_t sample_number) const
{
    return this->slice(0, this->fast_axis_length(), sample_number, sample_number + 1);
}
*/

/*
template<class Arch>
typename TimeFrequencyFlags<Arch>::ChannelRange TimeFrequencyFlags<Arch>::channel(unsigned channel_number)
{
    return this->slice(channel_number, channel_number+1, 0, number_of_samples());
}
*/

template<class Arch>
void TimeFrequencyFlags<Arch>::reset(bool v)
{
    std::fill(this->begin(), this->end(), v);
}

} // namespace data
} // namespace cheetah
} // namespace ska
