#ifndef SKA_CHEETAH_DATA_RFIMFLAGGEDDATA_H
#define SKA_CHEETAH_DATA_RFIMFLAGGEDDATA_H

#include "cheetah/data/TimeFrequencyFlags.h"
#include "cheetah/data/TimeFrequencyStats.h"
#include "panda/DataChunk.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief TimeFrequency data with flags representing rfim detection
 *
 */

template<typename TimeFrequencyType>
class RfimFlaggedData : public TimeFrequencyStats<TimeFrequencyType>
                      , public panda::DataChunk<RfimFlaggedData<TimeFrequencyType>>
{
        typedef TimeFrequencyStats<TimeFrequencyType> BaseT;

    public:
        typedef data::TimeFrequencyFlags<Cpu> TimeFrequencyFlagsType;
        typedef typename TimeFrequencyType::NumericalRep NumericalRep;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;

    public:
        RfimFlaggedData(TimeFrequencyType&);
        RfimFlaggedData(std::shared_ptr<TimeFrequencyType> ptr);
        template<typename Dim1, typename Dim2>
        RfimFlaggedData(data::DimensionSize<Dim1>, data::DimensionSize<Dim2>);
        ~RfimFlaggedData();

        operator TimeFrequencyType&();
        operator TimeFrequencyType const&() const;
        operator data::TimeFrequencyFlags<Cpu>&();

        data::TimeFrequencyFlags<Cpu> const& rfi_flags() const;
        data::TimeFrequencyFlags<Cpu>& rfi_flags();
        TimeFrequencyType& tf_data();
        TimeFrequencyType const& tf_data() const;


    private:
        TimeFrequencyFlagsType _flag_data;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "detail/RfimFlaggedData.cpp"

#endif // SKA_CHEETAH_DATA_RFIMFLAGGEDDATA_H
