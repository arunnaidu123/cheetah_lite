include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})


set(
    gtest_ddtr_klotski_src
    src/DdtrTest.cpp
    src/gtest_ddtr_klotski.cpp
    src/DedispersionStrategyTest.cpp
)

if(ENABLE_NASM)
    add_executable(gtest_ddtr_klotski ${gtest_ddtr_klotski_src})
    target_link_libraries(gtest_ddtr_klotski ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
    add_test(gtest_ddtr_klotski gtest_ddtr_klotski --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
endif(ENABLE_NASM)
