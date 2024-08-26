include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_cuda_utils_src_cpu src/gtest_cuda_utils.cpp)

set(gtest_cuda_utils_src_cuda)

#set(gtest_cuda_utils_src_cuda
#    add cuda unitittests foo.cu
#)

if(ENABLE_CUDA)
    cuda_add_executable(gtest_cuda_utils ${gtest_cuda_utils_src_cuda} ${gtest_cuda_utils_src_cpu})
else()
    add_executable(gtest_cuda_utils ${gtest_cuda_utils_src_cpu})
endif()

target_link_libraries(gtest_cuda_utils ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_cuda_utils gtest_cuda_utils)
