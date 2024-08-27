include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_spdt_klotski_bruteforce_src
    src/ConfigTest.cpp
    src/SpdtTest.cpp
    src/gtest_spdt_klotski_bruteforce.cpp
)

if(ENABLE_NASM)
add_executable(gtest_spdt_klotski_bruteforce ${gtest_spdt_klotski_bruteforce_src})
target_link_libraries(gtest_spdt_klotski_bruteforce ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_spdt_klotski_bruteforce gtest_spdt_klotski_bruteforce --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
endif(ENABLE_NASM)