#---------------------------------------------------------
# CUDA development toolkit and configuration
#
# Interface Variables:
# CUDA_NVCC_FLAGS  : additional flags to send to the nvcc compiler
#---------------------------------------------------------
if(PANDA_CMAKE_MODULE_PATH)
    include("${PANDA_CMAKE_MODULE_PATH}/cuda.cmake")
else()
    message(FATAL_ERROR "Dependency Error: include cuda.cmake after panda has been found")
endif()

if(ENABLE_CUDA)

    include_directories(${CUDA_TOOLKIT_INCLUDE})

    # Add useful headerfiles like cuda_helpers.h from sdk samples
    if(CUDA_SMP_INC)
        include_directories(${CUDA_SMP_INC})
    endif()

    if(CUDA_VERSION_MAJOR EQUAL "11")
        add_definitions(-DTHRUST_IGNORE_DEPRECATED_CPP_DIALECT)
    endif()

    list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_70,code=sm_70) # V100
    list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_86,code=sm_86) # A40
    list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_61,code=sm_61) # Titan 1080
    list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_75,code=sm_75) # Quadro

    option(CUDA_USE_NVTX "set to true to enable NVTX macros for CUDA profiling" OFF)
    if(CUDA_USE_NVTX)
        set(CUDA_USE_NVTX true)
        add_definitions(-DUSE_NVTX)
        find_library(CUDA_NVTOOLSEXT_LIB
                     NAMES nvToolsExt
                     HINTS ${CUDA_TOOLKIT_ROOT_DIR}/lib64
        )
        list(APPEND CUDA_DEPENDENCY_LIBRARIES ${CUDA_NVTOOLSEXT_LIB})
    else()
        set(CUDA_USE_NVTX false)
    endif()

endif()
