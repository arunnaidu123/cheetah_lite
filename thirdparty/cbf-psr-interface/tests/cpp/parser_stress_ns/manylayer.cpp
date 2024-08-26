#include <cstdint>

namespace parser_stress
{
    struct stress_0 { int8_t a; };
    struct stress_1 { stress_0 parent; };
    struct stress_2 { stress_1 parent; };
    struct stress_3 { stress_2 parent; };
    struct stress_4 { stress_3 parent; };
    struct stress_5 { stress_4 parent; };
    struct stress_6 { stress_5 parent; };
    struct stress_7 { stress_6 parent; };
    struct stress_8 { stress_7 parent; };
    struct stress_9 { stress_8 parent; };
    struct stress_a { stress_9 parent; };
    struct stress_b { stress_a parent; };
    struct stress_c { stress_b parent; };
    struct stress_d { stress_c parent; };
    struct stress_e { stress_d parent; };
    struct stress_f { stress_e parent; };
};

struct ignore_this
{
    int8_t should_not_appear_in_vhdl;
};
