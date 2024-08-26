set(MODULE_CANDIDATE_PIPELINE_LIB_SRC_CPU
    src/Config.cpp
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)

add_executable(cheetah_candidate_pipeline src/main.cpp)
target_link_libraries(cheetah_candidate_pipeline ${CHEETAH_LIBRARIES})
