include_directories(${GTEST_INCLUDE_DIR})

link_directories(${GTEST_LIBRARY_DIR})

set(gtest_corner_turn_cpu_src src/CornerTurnTest.cpp)
add_executable(gtest_corner_turn_cpu ${gtest_corner_turn_cpu_src} )
target_link_libraries(gtest_corner_turn_cpu ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_corner_turn_cpu gtest_corner_turn_cpu --test_corner_turn "${CMAKE_CURRENT_LIST_DIR}/corner_turn")
