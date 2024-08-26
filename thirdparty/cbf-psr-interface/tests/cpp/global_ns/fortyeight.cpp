#include <cstdint>

// I don't think the parser actually uses the pragma,
// but it's needed to get the correct behaviour in a real compiler.
#pragma pack(1)

namespace cbf_psr_interface
{
    struct fortyeight
    {
        uint64_t test : 48; // 48-bit field test
    };
};
