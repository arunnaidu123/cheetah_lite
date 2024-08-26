include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_generators_src
    src/GaussianNoiseTest.cpp
    src/SimplePulsarTest.cpp
    src/PulsarInjectionConfigTest.cpp
    src/PulsarInjectionTest.cpp
    src/Tempo2PhaseModelTest.cpp
    src/gtest_generators.cpp
)

add_executable(gtest_generators ${gtest_generators_src})
target_link_libraries(gtest_generators ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_generators gtest_generators --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
