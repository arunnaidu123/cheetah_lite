include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_spdt_src src/gtest_spdt.cpp)

add_executable(gtest_spdt ${gtest_spdt_src})
target_link_libraries(gtest_spdt ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_spdt gtest_spdt --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
