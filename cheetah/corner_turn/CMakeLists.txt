subpackage(cpu)
subpackage(nasm)
subpackage(cuda)

set(MODULE_CORNER_TURN_LIB_SRC_CPU
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_CORNER_TURN_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

test_utils()

add_subdirectory(test)
