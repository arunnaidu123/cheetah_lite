include(subpackage)

# Define the cheetah libraries
set(CHEETAH_LIBRARIES ${PROJECT_NAME} ${DEPENDENCY_LIBRARIES})

# Define the cheetah test_utils libraries
set(CHEETAH_TEST_UTILS ${PROJECT_NAME}_test_utils)
list(APPEND CHEETAH_TEST_UTILS ${PANDA_TEST_LIBRARIES})

# Ensure we can access our headers with #include "cheetah/...."
include_directories(..)
include_directories(${PROJECT_BINARY_DIR})

# Subpackages to define ${LIB_SRC} & ${TEST_UTILS_SRC} lists
# Included in dependency order
subpackage(utils) # not dependent on any other subpackage
subpackage(channel_mask)
subpackage(channel_stats)
subpackage(cuda_utils)
subpackage(corner_turn)
subpackage(data)
subpackage(sigproc)
subpackage(psrdada)
subpackage(emulator)
subpackage(generators)
subpackage(io)
subpackage(modules)
subpackage(pipelines)

# Should come after all subpackage() directives
include_subpackage_files()

# The main cuda library
if(ENABLE_CUDA)
    cuda_compile(CUDA_OBJECTS ${LIB_SRC_CUDA})
    list(APPEND CUDA_OBJECTS ${LIB_OBJ_CUDA})
    cuda_generate_link_file(cuda_link_file ${PROJECT_NAME} ${CUDA_OBJECTS})
    list(APPEND CUDA_OBJECTS ${CUDA_LINK_FILE})
endif()

# The main library target
add_library(${PROJECT_NAME} ${LIB_SRC_CPU} ${CUDA_OBJECTS})

if(ENABLE_CUDA)
    cuda_add_cufft_to_target(${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES})
    # We need to set the linker language based on what the expected generated file
    # would be. CUDA_C_OR_CXX is computed based on CUDA_HOST_COMPILATION_CPP.
    set_target_properties(${cuda_target}
                          PROPERTIES
                          LINKER_LANGUAGE ${CUDA_C_OR_CXX}
    )
endif()

# test_utils library target
add_library(${PROJECT_NAME}_test_utils ${TEST_UTILS_SRC_CPU})
target_include_directories(${PROJECT_NAME}_test_utils PUBLIC ${GTEST_INCLUDE_DIR})

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_test_utils DESTINATION ${LIBRARY_INSTALL_DIR})
