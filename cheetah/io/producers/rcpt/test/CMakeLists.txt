include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_rcpt_src src/gtest_rcpt.cpp)

add_executable(gtest_rcpt ${gtest_rcpt_src})
target_link_libraries(gtest_rcpt ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_rcpt gtest_rcpt --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
