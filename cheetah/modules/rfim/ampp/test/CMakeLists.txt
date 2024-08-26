include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_rfim_ampp_src
    src/AmppTest.cpp
    src/gtest_rfim_ampp.cpp
)

add_executable(gtest_rfim_ampp ${gtest_rfim_ampp_src})
target_link_libraries(gtest_rfim_ampp ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
#add_test(gtest_rfim_ampp gtest_rfim_ampp --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
