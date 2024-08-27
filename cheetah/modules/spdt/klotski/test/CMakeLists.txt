include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_spdt_klotski_src
    src/ConfigTest.cpp
    src/SpdtTest.cpp
    src/gtest_spdt_klotski.cpp
)

if(ENABLE_NASM)
add_executable(gtest_spdt_klotski ${gtest_spdt_klotski_src})
target_link_libraries(gtest_spdt_klotski ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_spdt_klotski gtest_spdt_klotski --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
endif(ENABLE_NASM)