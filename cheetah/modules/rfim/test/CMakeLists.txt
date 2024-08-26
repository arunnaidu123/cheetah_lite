include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_rfim_src
    ${GTEST_RFIM_POLICY_SRC}
    src/MetricsTest.cpp
    src/FlaggedDataReplacerTest.cpp
    src/gtest_rfim.cpp
)

add_executable(gtest_rfim ${gtest_rfim_src})
target_link_libraries(gtest_rfim ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_rfim gtest_rfim --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
