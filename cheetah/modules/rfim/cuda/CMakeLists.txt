set(MODULE_CUDA_LIB_SRC_CPU
    src/Config.cpp
    PARENT_SCOPE
)

set(MODULE_CUDA_LIB_SRC_CUDA
    src/Rfim.cu
    PARENT_SCOPE
)

add_subdirectory(test)
