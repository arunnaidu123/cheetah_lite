include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

if(ENABLE_NASM)
    set(gtest_corner_turn_nasm_src src/CornerTurnTest.cpp)
    add_executable(gtest_corner_nasm_turn ${gtest_corner_turn_nasm_src} )
    target_link_libraries(gtest_corner_nasm_turn ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
    add_test(gtest_corner_nasm_turn gtest_corner_nasm_turn --test_corner_turn "${CMAKE_CURRENT_LIST_DIR}/data")
endif()
