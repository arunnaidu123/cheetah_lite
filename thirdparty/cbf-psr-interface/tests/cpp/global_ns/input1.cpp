#include <cstdint>

const int arraysize=5;

struct packet0 {
    uint8_t             counter;
    int64_t  data[arraysize];
}  ;

struct packet1 {
    uint16_t counter;
    int32_t data[arraysize];
    // function should be ignored in VHDL output
    int some_function() { return 1; };
    int other_function() const { return 200; };
};
