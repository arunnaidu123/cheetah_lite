include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(gtest_pipeline_src
    src/BeamConfigTest.cpp
    src/BeamLauncherTest.cpp
    src/CheetahConfigTest.cpp
    src/DataExportTest.cpp
    src/MultiBeamConfigTest.cpp
    src/PipelineHandlerFactoryTest.cpp
    src/RfiDetectionPipelineTest.cpp
    src/SinglePulseTest.cpp
    src/gtest_pipeline.cpp
)

add_executable(gtest_pipeline ${gtest_pipeline_src})
target_link_libraries(gtest_pipeline ${CHEETAH_TEST_UTILS} ${CHEETAH_LIBRARIES} ${GTEST_LIBRARIES})
add_test(gtest_pipeline gtest_pipeline)
