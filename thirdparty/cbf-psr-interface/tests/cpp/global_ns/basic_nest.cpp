#include <cstdint>

const int payload_size = 1000; // bytes

struct sample
{
    uint8_t  x;
    uint8_t  y;
};

const int n_samples = payload_size / sizeof(sample);

struct nested
{
    uint16_t meta;
    sample spare;
    sample data[n_samples];
};
