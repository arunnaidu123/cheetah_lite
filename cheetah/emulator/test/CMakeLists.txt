include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_emulator_src src/gtest_emulator.cpp)

add_executable(gtest_emulator ${gtest_emulator_src})
target_link_libraries(gtest_emulator ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_emulator gtest_emulator --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
