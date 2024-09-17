include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_ddtr_src
    src/AggregationBufferTest.cpp
    src/AggregationBufferFillerTest.cpp
    src/BufferingTest.cpp
    src/CommonDedispersionPlanTest.cpp
    src/DedispersionTrialPlanTest.cpp
    src/DdtrConfigTest.cpp
    src/TimeFrequencyBufferFactoryTest.cpp
    src/RfiExcisionFactoryTest.cpp
    src/gtest_ddtr.cpp
)

add_executable(gtest_ddtr ${gtest_ddtr_src})
target_link_libraries(gtest_ddtr ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_ddtr gtest_ddtr)
