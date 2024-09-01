subpackage(cpu)

set(MODULE_SPDT_LIB_SRC_CPU
    src/Config.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_SPDT_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
