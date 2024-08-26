include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_nasm_src src/gtest_nasm.cpp)
add_executable(gtest_nasm ${gtest_nasm_src})
target_link_libraries(gtest_nasm ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_nasm gtest_nasm --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
