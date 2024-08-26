include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_sigproc_src
    src/SigProcFileStreamTest.cpp
    src/SigProcHeaderTest.cpp
    src/SigProcWriterTest.cpp
    src/gtest_sigproc.cpp
)

add_executable(gtest_sigproc ${gtest_sigproc_src})
target_link_libraries(gtest_sigproc ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_sigproc gtest_sigproc --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
