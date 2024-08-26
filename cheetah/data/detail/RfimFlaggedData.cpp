#include "cheetah/data/RfimFlaggedData.h"


namespace ska {
namespace cheetah {
namespace data {


template<typename TimeFrequencyType>
RfimFlaggedData<TimeFrequencyType>::RfimFlaggedData(std::shared_ptr<TimeFrequencyType> ptr)
    : BaseT(std::move(ptr))
    , _flag_data(this->get().template dimension<data::Time>(), ptr->template dimension<data::Frequency>())
{
    std::fill(_flag_data.begin(), _flag_data.end(), false);
}

template<typename TimeFrequencyType>
RfimFlaggedData<TimeFrequencyType>::RfimFlaggedData(TimeFrequencyType& data)
    : BaseT(data.shared_from_this())
    , _flag_data(data.template dimension<data::Time>(), data.template dimension<data::Frequency>())
{
    std::fill(_flag_data.begin(), _flag_data.end(), false);
}

template<typename TimeFrequencyType>
template<typename Dim1, typename Dim2>
RfimFlaggedData<TimeFrequencyType>::RfimFlaggedData(data::DimensionSize<Dim1> d1, data::DimensionSize<Dim2> d2)
    : BaseT(std::make_shared<TimeFrequencyType>(d1, d2))
    , _flag_data(this->get().template dimension<data::Time>()
               , this->get().template dimension<data::Frequency>())
{
    std::fill(_flag_data.begin(), _flag_data.end(), false);
}

template<typename TimeFrequencyType>
RfimFlaggedData<TimeFrequencyType>::~RfimFlaggedData()
{
}

template<typename TimeFrequencyType>
RfimFlaggedData<TimeFrequencyType>::operator TimeFrequencyType&()
{
    return this->get();
}

template<typename TimeFrequencyType>
RfimFlaggedData<TimeFrequencyType>::operator TimeFrequencyType const&() const
{
    return this->get();
}

template<typename TimeFrequencyType>
RfimFlaggedData<TimeFrequencyType>::operator data::TimeFrequencyFlags<Cpu>&()
{
    return _flag_data;
}

template<typename TimeFrequencyType>
data::TimeFrequencyFlags<Cpu> const& RfimFlaggedData<TimeFrequencyType>::rfi_flags() const
{
    return _flag_data;
}

template<typename TimeFrequencyType>
data::TimeFrequencyFlags<Cpu>& RfimFlaggedData<TimeFrequencyType>::rfi_flags()
{
    return _flag_data;
}


template<typename TimeFrequencyType>
TimeFrequencyType& RfimFlaggedData<TimeFrequencyType>::tf_data()
{
    return this->get();
}

template<typename TimeFrequencyType>
TimeFrequencyType const& RfimFlaggedData<TimeFrequencyType>::tf_data() const
{
    return this->get();
}

} // namespace data
} // namespace cheetah
} // namespace ska

namespace pss {
namespace astrotypes {

template<typename TimeFrequencyT, typename...Dims>
struct has_exact_dimensions<ska::cheetah::data::RfimFlaggedData<TimeFrequencyT>, Dims...> : public has_exact_dimensions<TimeFrequencyT, Dims...>
{
};

} // namespace pss
} // namespace astrotypes
