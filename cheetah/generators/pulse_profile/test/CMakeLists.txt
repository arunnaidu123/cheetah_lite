include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_pulse_profile_src
    src/ProfileManagerTest.cpp
    src/PulsarProfileTest.cpp
    src/gtest_pulse_profile.cpp
)

add_executable(gtest_pulse_profile ${gtest_pulse_profile_src})
target_link_libraries(gtest_pulse_profile ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_pulse_profile gtest_pulse_profile --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
