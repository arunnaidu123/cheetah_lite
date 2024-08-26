include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_rcpt_low_src
    src/gtest_rcpt_low.cpp
    src/BeamFormerPacketTest.cpp
    src/BeamFormerPacketInspectorTest.cpp
    src/BeamFormerDataTraitsTest.cpp
    src/UdpStreamFrequencyTimeTest.cpp
    src/UdpStreamTimeFrequencyTest.cpp
)

add_executable(gtest_rcpt_low ${gtest_rcpt_low_src})
target_link_libraries(gtest_rcpt_low ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_rcpt_low gtest_rcpt_low --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
