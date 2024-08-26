#include <cstdint>

const int arraysize=11;

struct packet2
{
    uint8_t  u_counter;
    int8_t  counter;
    int64_t  data[arraysize];
    uint64_t  u_data[arraysize];
};

struct packet3
{
    uint16_t u_counter;
    int16_t counter;
    int32_t data[arraysize];
    uint32_t u_data[arraysize];
};
