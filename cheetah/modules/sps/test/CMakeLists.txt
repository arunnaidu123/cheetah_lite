include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_sps_src src/gtest_sps.cpp)

add_executable(gtest_sps ${gtest_sps_src})
target_link_libraries(gtest_sps ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_sps gtest_sps --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
