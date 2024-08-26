#ifndef SKA_CHEETAH_DATA_TIMEFREQUENCYFLAGS_H
#define SKA_CHEETAH_DATA_TIMEFREQUENCYFLAGS_H

#include "cheetah/data/DataSequence2D.h"
#include "panda/DataChunk.h"
#include "pss/astrotypes/types/TimeFrequency.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

using pss::astrotypes::units::Frequency;
using pss::astrotypes::units::Time;
template<typename T> using DimensionSize = pss::astrotypes::DimensionSize<T>;
typedef bool FlagsType;

template<class Arch=Cpu>
class TimeFrequencyFlags : public pss::astrotypes::TimeFrequency<uint8_t>
                         , public panda::DataChunk<TimeFrequencyFlags<Arch>>
{
        typedef pss::astrotypes::TimeFrequency<uint8_t> BaseT;

   public:
        using typename BaseT::Channel;
        using typename BaseT::ConstChannel;

    public:
        /**
         * Construct an empty TimeFrequencyFlags object
         */
        TimeFrequencyFlags();

        /**
         * Construct a defined size TimeFrequencyFlags object
         */
        TimeFrequencyFlags(DimensionSize<Time> number_of_spectra, DimensionSize<Frequency> number_of_channels);
        ~TimeFrequencyFlags();

        /**
         * @returns the integer number of channels in the block
         */
        std::size_t number_of_channels() const;

        /**
         * @returns the integer number of samples in the block
         */
        std::size_t number_of_spectra() const;

        /**
         * @brief reset all flags to the dspecified value
         */
        void reset(bool v = false);
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "detail/TimeFrequencyFlags.cpp"

#endif // SKA_CHEETAH_DATA_TIMEFREQUENCYFLAGS_H
