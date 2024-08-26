subpackage(welford)
subpackage(cpu)
subpackage(nasm)

set(MODULE_CHANNEL_STATS_LIB_SRC_CPU
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_CHANNEL_STATS_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

add_subdirectory(test)
