include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_corner_turn_src
    src/CornerTurnTest.cpp
    src/gtest_corner_turn.cpp
)
add_executable(gtest_corner_turn ${gtest_corner_turn_src} )
target_link_libraries(gtest_corner_turn ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_corner_turn gtest_corner_turn --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
