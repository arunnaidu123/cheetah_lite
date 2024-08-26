include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_exporters_src
    src/DataExportTest.cpp
    src/DataExportConfigTest.cpp
    src/OcldFileStreamerTest.cpp
    src/SclFileStreamerTest.cpp
    src/SpCandidateDataStreamerTest.cpp
    src/SclCandidateDataStreamerTest.cpp
    src/SpCclFileStreamerTest.cpp
    src/SpCclSigProcTest.cpp
    src/gtest_exporters.cpp
)

if(SPEAD_FOUND)
    list(APPEND gtest_exporters_src
                src/SpCclSpeadStreamerTest.cpp
                src/SpCclSpeadReaderTest.cpp
    )
endif()

add_executable(gtest_exporters ${gtest_exporters_src})
target_link_libraries(gtest_exporters ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_exporters gtest_exporters --test_data "${CMAKE_CURRENT_LIST_DIR}/data")
