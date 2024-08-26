include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_welford_src src/gtest_welford.cpp)
add_executable(gtest_welford ${gtest_welford_src})
target_link_libraries(gtest_welford ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_welford gtest_welford --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
