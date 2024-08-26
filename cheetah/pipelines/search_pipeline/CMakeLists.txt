set(MODULE_SEARCH_PIPELINE_LIB_SRC_CPU
    src/EmptyConfig.cpp
    src/PipelineHandlerFactory.cpp
    src/ScanConfig.cpp
    PARENT_SCOPE
)

add_executable(cheetah_pipeline src/main.cpp)
install(TARGETS cheetah_pipeline DESTINATION ${BINARY_INSTALL_DIR})
target_link_libraries(cheetah_pipeline ${CHEETAH_LIBRARIES})

test_utils()
add_subdirectory(test)
