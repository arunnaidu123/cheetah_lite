include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_candidate_pipeline_src src/gtest_candidate_pipeline.cpp)

add_executable(gtest_candidate_pipeline ${gtest_candidate_pipeline_src})
target_link_libraries(gtest_candidate_pipeline ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_candidate_pipeline gtest_candidate_pipeline --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
