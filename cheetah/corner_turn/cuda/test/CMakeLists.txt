include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_corner_turn_cuda_src src/CornerTurnTest.cu)

if(ENABLE_CUDA)
    cuda_compile(cuda_objects ${gtest_corner_turn_cuda_src})
    cuda_generate_link_file(cuda_link_file ${cuda_objects})
    add_executable(gtest_corner_turn_cuda ${gtest_corner_turn_cuda_src} ${cuda_objects} ${gtest_corner_turn_src})
    target_link_libraries(gtest_corner_turn_cuda ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
    add_test(gtest_corner_turn_cuda gtest_corner_turn_cuda --test_corner_turn "${CMAKE_CURRENT_LIST_DIR}/corner_turn")
endif()
