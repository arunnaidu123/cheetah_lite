include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_data_cuda_src
    src/CachingAllocatorTest.cu
    src/DmTimeDmTest.cu
    src/DmTrialsTest.cu
    src/FrequencySeriesTest.cu
    src/FrequencyTimeTest.cu
    src/PowerSeriesTest.cu
    src/SeriesTest.cu
    src/TimeFrequencyTest.cu
    src/TimeSeriesTest.cu
    src/gtest_data_cuda.cu
)

set(gtest_data_cpu_src)

if(ENABLE_CUDA)
    cuda_compile(CUDA_OBJECTS ${gtest_data_cuda_src})
    cuda_generate_link_file(cuda_link_file ${CUDA_OBJECTS})
    add_executable(gtest_data_cuda ${gtest_data_cpu_src} ${CUDA_OBJECTS} ${gtest_data_src})
    target_link_libraries(gtest_data_cuda ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
    add_test(gtest_data_cuda gtest_data_cuda --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
endif()
