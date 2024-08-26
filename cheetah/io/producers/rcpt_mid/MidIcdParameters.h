#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_MIDICDPARAMETERS_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_MIDICDPARAMETERS_H

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

struct PssMidTraits
{
    static constexpr uint32_t number_of_channels = 3700;
    static constexpr uint32_t number_of_channels_per_packet = 185;
    static constexpr uint32_t number_of_samples_per_packet = 8;
    static constexpr uint32_t size_of_packet = 6400;

    typedef uint8_t PacketDataType;
};

} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_MIDICDPARAMETERS_H
