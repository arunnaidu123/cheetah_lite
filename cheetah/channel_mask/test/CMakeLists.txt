include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_channel_mask_src
    src/ChannelMaskTest.cpp
    src/ConfigurableChannelMaskTest.cpp
    src/PolicyFactoryTest.cpp
    src/ReplaceConstantPolicyTest.cpp
    src/gtest_channel_mask.cpp
)

add_executable(gtest_channel_mask ${gtest_channel_mask_src})
target_link_libraries(gtest_channel_mask ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_channel_mask gtest_channel_mask --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
