include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_rfim_cuda_src
#    src/CudaTest.cpp
    src/gtest_rfim_cuda.cpp
)

add_executable(gtest_rfim_cuda ${gtest_rfim_cuda_src})
target_link_libraries(gtest_rfim_cuda ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_rfim_cuda gtest_rfim_cuda --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
