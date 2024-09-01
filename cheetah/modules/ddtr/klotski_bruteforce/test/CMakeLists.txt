include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})


set(
    gtest_ddtr_klotski_bruteforce_src
    src/DdtrTest.cpp
    src/gtest_ddtr_klotski_bruteforce.cpp
    src/DedispersionStrategyTest.cpp
)

if(ENABLE_NASM)
    add_executable(gtest_ddtr_klotski_bruteforce ${gtest_ddtr_klotski_bruteforce_src})
    target_link_libraries(gtest_ddtr_klotski_bruteforce ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
    add_test(gtest_ddtr_klotski_bruteforce gtest_ddtr_klotski_bruteforce --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
endif(ENABLE_NASM)
