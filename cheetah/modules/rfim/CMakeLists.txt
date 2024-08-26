subpackage(policy)
subpackage(ampp)
subpackage(cuda)
subpackage(sum_threshold)
subpackage(iqrmcpu)

set(MODULE_RFIM_LIB_SRC_CPU
    src/Config.cpp
    src/Metrics.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_RFIM_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

test_utils()

foreach(src ${GTEST_RFIM_POLICY_SRC_STEM})
    list(APPEND GTEST_RFIM_POLICY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/policy/test/${src}")
endforeach()
add_subdirectory(test)