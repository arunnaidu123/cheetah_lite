include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_spsift_src
    src/gtest_spsift.cpp
    src/SpSiftTest.cpp
)

add_executable(gtest_spsift ${gtest_spsift_src})
target_link_libraries(gtest_spsift ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_spsift gtest_spsift --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
