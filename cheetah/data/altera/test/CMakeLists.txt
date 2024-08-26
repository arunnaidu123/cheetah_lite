include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_data_altera_src
    src/SeriesTest.cpp
    src/FrequencySeriesTest.cpp
    src/gtest_data_altera.cpp
)

if(ENABLE_OPENCL)
    add_executable(gtest_data_altera ${gtest_data_altera_src})
    target_link_libraries(gtest_data_altera ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
    add_test(gtest_data_altera gtest_data_altera --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
endif()
