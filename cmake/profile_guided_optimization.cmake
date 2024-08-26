# Profile Guided Optimization

if(NOT SKA_CHEETAH_PROFILE_GUIDED_OPTIMIZATION_GUARD_VAR)
    set(SKA_CHEETAH_PROFILE_GUIDED_OPTIMIZATION_GUARD_VAR TRUE)
else()
    return()
endif()

if(CMAKE_BUILD_TYPE MATCHES pgotrain)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(pgo_compile_flags "-fprofile-generate=${CMAKE_BINARY_DIR}/profile-data")
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
        set(pgo_compile_flags "-fprofile-instr-generate -g -O0  -fprofile-arcs -ftest-coverage")
    endif()

    # Add the CMAKE_CXX_FLAGS_RELEASE so that a PGO-optimized build also includes release flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${pgo_compile_flags}")
endif()

if(CMAKE_BUILD_TYPE MATCHES pgobuild)

    # Where to find the profiling data from the training run
    if(NOT PGO_TRAINING_DIR)
        set(PGO_TRAINING_DIR ../training)
    endif()
    set(pgo_training_data ${CMAKE_BINARY_DIR}/${PGO_TRAINING_DIR}/profile-data)

    if(NOT EXISTS ${pgo_training_data})
        message(FATAL_ERROR "No profiling data found, so can't build. Ensure that the training run was executed in the training build directory. Training data expected in directory: " ${pgo_training_data})
    endif()

    if(CMAKE_COMPILER_IS_GNUCXX)
        set(pgo_compile_flags "-fprofile-use=${pgo_training_data} -fprofile-correction")
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
        set(pgo_compile_flags "-fprofile-instr-use -g -O0  -fprofile-arcs -ftest-coverage")
    endif()

    # This custom target always runs.
    # It launches two commands which will run in the directory specified by PGO_TRAINING_DIR.
    # First, it runs 'make all' in the profile instrumented build area
    # Next it runs 'make test' to ensure that the profiling information is generated.
    # In this way, running 'make all' in the final build area guarantees that the profile
    # instrumented training files will be re-compiled, then the test suite will be run
    # to generate new profiling files, before the final build version is compiled using
    # this profiling information.
    add_custom_target(run_training
                      ALL
                      WORKING_DIRECTORY ${PGO_TRAINING_DIR}
                      COMMAND ${CMAKE_BUILD_TOOL} all
                      COMMAND ${CMAKE_BUILD_TOOL} test
                      VERBATIM
    )

    # Add the CMAKE_CXX_FLAGS_RELEASE so that a PGO-optimized build also includes release flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${pgo_compile_flags}")

endif()
