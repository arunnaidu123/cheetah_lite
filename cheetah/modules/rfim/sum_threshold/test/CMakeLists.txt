include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_rfim_sum_threshold_src
    src/SumThresholdTest.cpp
    src/gtest_rfim_sum_threshold.cpp
)

add_executable(gtest_rfim_sum_threshold ${gtest_rfim_sum_threshold_src})
target_link_libraries(gtest_rfim_sum_threshold ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
#add_test(gtest_rfim_sum_threshold gtest_rfim_sum_threshold --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
