include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_sps_cpu_src
    src/ConfigTest.cpp
    src/SpsTest.cpp
    src/gtest_sps_cpu.cpp
)

add_executable(gtest_sps_cpu ${gtest_sps_cpu_src})
target_link_libraries(gtest_sps_cpu ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_sps_cpu gtest_sps_cpu --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
