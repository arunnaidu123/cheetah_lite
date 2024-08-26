include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_data_src
    src/BirdieTest.cpp
    src/CachingAllocatorTest.cpp
    src/CandidateTest.cpp
    src/CclTest.cpp
    src/DataSequence2DTest.cpp
    src/DataSequence2DFastIteratorTest.cpp
    src/DedispersionMeasureTest.cpp
    src/DmTimeTest.cpp
    src/DmTrialsTest.cpp
    src/FrequencySeriesTest.cpp
    src/NuDotTest.cpp
    src/OcldTest.cpp
    src/OptimisedCandidateTest.cpp
    src/PowerSeriesTest.cpp
    src/RfimFlaggedDataTest.cpp
    src/SeriesTest.cpp
    src/SclTest.cpp
    src/SpCandidateTest.cpp
    src/SpCclTest.cpp
    src/TimeFrequencyTest.cpp
    src/TimeFrequencyMetadataTest.cpp
    src/TimeFrequencyFlagsTest.cpp
    src/TimeFrequencyContainerTest.cpp
    src/TimeFrequencyStatsTest.cpp
    src/TimeSeriesTest.cpp
    src/UnitsTest.cpp
    src/VectorLikeTest.cpp
    src/FrequencyTimeTest.cpp
    src/gtest_data.cpp
)

set(gtest_data_src_cuda)

add_executable(gtest_data ${gtest_data_src})
target_link_libraries(gtest_data ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_data gtest_data --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
