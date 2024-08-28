subpackage(cpu)
subpackage(klotski_common)
subpackage(klotski_bruteforce)
subpackage(klotski)

set(MODULE_SPS_LIB_SRC_CPU
    src/Config.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_SPS_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

#test_utils()
#add_subdirectory(test)
