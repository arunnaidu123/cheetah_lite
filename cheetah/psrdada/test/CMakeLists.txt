include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_psrdada_src
    src/DadaReadWriteClientTest.cpp
    src/MultiLogTest.cpp
    src/RawBytesTest.cpp
    src/SigProcDadaStreamTest.cpp
    src/gtest_psrdada.cpp
)

add_executable(gtest_psrdada ${gtest_psrdada_src})
target_link_libraries(gtest_psrdada ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_psrdada gtest_psrdada --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
