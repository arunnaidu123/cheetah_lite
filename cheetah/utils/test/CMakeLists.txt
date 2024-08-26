include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_utils_src
    src/AlgoModuleTest.cpp
    src/AlgorithmTesterTest.cpp
    src/BinMapTest.cpp
    src/ConvolvePlanTest.cpp
    src/JulianClockTest.cpp
    src/ModifiedJulianClockTest.cpp
    src/TaskConfigurationSetterTest.cpp
    src/gtest_utils.cpp
)

add_executable(gtest_utils ${gtest_utils_src})
target_link_libraries(gtest_utils ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_utils gtest_utils)
