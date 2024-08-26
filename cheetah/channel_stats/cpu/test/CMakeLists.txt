include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_cpu_src src/gtest_cpu.cpp)
add_executable(gtest_cpu ${gtest_cpu_src})
target_link_libraries(gtest_cpu ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_cpu gtest_cpu --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
