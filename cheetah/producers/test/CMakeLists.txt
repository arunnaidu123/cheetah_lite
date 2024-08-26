include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

if(SPEAD_FOUND)
    set(spead_lib_src_cpu src/SpCclSpeadProducerTest.cpp)
endif()

set(gtest_producers_src
    ${spead_lib_src_cpu}
    src/gtest_producers.cpp
)

add_executable(gtest_producers ${gtest_producers_src})
target_link_libraries(gtest_producers ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_producers gtest_producers --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
