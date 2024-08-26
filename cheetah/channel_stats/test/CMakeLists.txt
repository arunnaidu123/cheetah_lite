include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_channel_stats_src src/gtest_channel_stats.cpp)
add_executable(gtest_channel_stats ${gtest_channel_stats_src})
target_link_libraries(gtest_channel_stats ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_channel_stats gtest_channel_stats --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
