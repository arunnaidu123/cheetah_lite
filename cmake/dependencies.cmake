if(NOT SKA_CHEETAH_DEPENDENCIES_GUARD_VAR)
    set(SKA_CHEETAH_DEPENDENCIES_GUARD_VAR TRUE)
else()
    return()
endif()

include(cmake/thirdparty.cmake)
set(THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/thirdparty")
set(THIRDPARTY_BINARY_DIR "${CMAKE_BINARY_DIR}/thirdparty")

include(doxygen)

if(CMAKE_BUILD_TYPE MATCHES documentation)
    #========================================================
    # message_active_deps
    #
    # brief: dummy function; only building the documentation,
    #        so only doc dependencies are required
    #
    # usage: message_active_deps(${DEPENDENCY_LIST})
    #========================================================
    function(message_active_deps)
    endfunction(message_active_deps)
else()

    include(compiler_settings)
    include(cmake/googletest.cmake)

    find_package(Panda REQUIRED)
    list(APPEND CMAKE_MODULE_PATH "${PANDA_CMAKE_MODULE_PATH}")
    include(dependency_register)

    include("${PANDA_CMAKE_MODULE_PATH}/pandaConfiguration.cmake")
    load_dependency(cuda) # Required to define some fake cmake macros if not enabled

    required_dependency(boost)

    # Third-party dependencies
    find_package(CbfPsrInterface REQUIRED)
    include_directories(SYSTEM ${CBF_PSR_INTERFACE_INCLUDE_DIR})
    find_package(AstroTypes REQUIRED) # Depends on boost
    include_directories(SYSTEM ${ASTROTYPES_INCLUDE_DIR})

    # External dependencies
    required_dependency(FFTW)
    optional_dependency(cuda)
    optional_dependency(Nasm)
    optional_dependency(Astroaccelerate)
    optional_dependency(PSRDADA)
    optional_dependency(Spead2)
    optional_dependency(SkaRabbit)

    include_directories(SYSTEM ${BOOST_INCLUDE_DIRS}
                               ${PANDA_INCLUDE_DIR}
    )

    if(FFTW_FOUND)
        include_directories(SYSTEM ${FFTW_INCLUDE_DIR})
    endif()
    if(ASTROACCELERATE_FOUND)
        include_directories(SYSTEM ${ASTROACCELERATE_INCLUDE_DIRS})
    endif()
    if(SPEAD_FOUND)
        include_directories(SYSTEM ${SPEAD_INCLUDE_DIR})
    endif()
    if(PSRDADA_FOUND)
        include_directories(SYSTEM ${PSRDADA_INCLUDE_DIRS})
    endif()
    if(SKA_RABBIT_FOUND)
        include_directories(SYSTEM ${SKA_RABBIT_INCLUDE_DIRS})
    endif()
    if(OPENCL_FOUND)
        include_directories(SYSTEM ${OPENCL_INCLUDE_DIR})
    endif()

    set(DEPENDENCY_LIBRARIES
        ${PANDA_LIBRARIES}
#        ${ASTROTYPES_LIBRARIES}
        ${SPEAD_LIBRARIES}
        ${BOOST_LIBRARIES}
        ${FFTW_LIBRARIES}
        ${ASTROACCELERATE_LIBRARIES}
        ${PSRDADA_LIBRARIES}
        ${CUDA_DEPENDENCY_LIBRARIES}
        ${OPENCL_LIBRARIES}
        ${SKA_RABBIT_LIBRARIES}
    )

    # Generate dependency-dependent artifacts
    generate_configuration_header("SKA_CHEETAH_")

endif()
