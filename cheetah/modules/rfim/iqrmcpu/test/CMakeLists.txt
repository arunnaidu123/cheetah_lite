include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_iqrm_src
    src/IqrmImplTest.cpp
    src/IqrmCpuTest.cpp
    src/gtest_iqrm.cpp
)

add_executable(gtest_iqrm ${gtest_iqrm_src})
target_link_libraries(gtest_iqrm ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_iqrm gtest_iqrm --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
