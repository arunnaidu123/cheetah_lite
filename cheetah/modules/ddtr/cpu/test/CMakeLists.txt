include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_ddtr_cpu_src
    src/DdtrTest.cpp
    src/gtest_ddtr_cpu.cpp
)

add_executable(gtest_ddtr_cpu ${gtest_ddtr_cpu_src})
target_link_libraries(gtest_ddtr_cpu ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_ddtr_cpu gtest_ddtr_cpu --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
