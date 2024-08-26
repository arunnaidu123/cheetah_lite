#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_LOWICDPARAMETERS_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_LOWICDPARAMETERS_H

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {

// TODO Move to the cbf-pss repository
struct PssLowTraits
{
    static constexpr uint32_t number_of_channels = 7776;
    static constexpr uint32_t number_of_channels_per_packet = 9;
    static constexpr uint32_t number_of_samples_per_packet = 128;
    static constexpr uint32_t size_of_packet = 4736;
    typedef int8_t PacketDataType;
};

} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_LOW_MIDICDPARAMETERS_H
