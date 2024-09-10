include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_sps_clustering_src
    src/FofTest.cpp
    src/SpsClusteringTest.cpp
    src/gtest_sps_clustering.cpp
)

add_executable(gtest_sps_clustering ${gtest_sps_clustering_src})
target_link_libraries(gtest_sps_clustering ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_sps_clustering gtest_sps_clustering --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
