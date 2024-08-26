#=======================================================================
# FindSkaRabbit
# -------------
#
# Find the SKA rabbit includes and library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will set the following variables:
#
#     ``SKA_RABBIT_FOUND``
#       TRUE if rabbit found.
#
#     ``SKA_RABBIT_INCLUDE_DIRS``
#       Directories where rabbit includes were found.
#
#     ``SKA_RABBIT_LIBRARIES``
#       List of libraries when using rabbit.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to explicitly specify the locations of rabbit components:
#
#     ``SKA_RABBIT_INSTALL_DIR``
#       Where rabbit framwork has been installed (lib/ and include/ directories included).
#
#     ``SKA_RABBIT_LIBRARY_DIRS``
#       Explicitly define in which directories rabbit libraries may be found.
#
#     ``SKA_RABBIT_INCLUDE_DIR``
#       Explicitly define in which directory rabbit includes may be found.
#
#=======================================================================
option(ENABLE_SKA_RABBIT "Enable modules that utilise the SKA rabbit library (FPGA images)" FALSE)

if(NOT SKA_CHEETAH_RABBIT_GUARD_VAR)
    set(SKA_CHEETAH_RABBIT_GUARD_VAR TRUE)
else()
    return()
endif()


if(ENABLE_SKA_RABBIT)

    # Required dependencies for SKA rabbit
    required_dependency(opencl)

    if(NOT OPENCL_FOUND)
        message(FATAL_ERROR "You must compile panda with -DENABLE_OPENCL")
    endif()

    if(SKA_RABBIT_INCLUDE_DIR)
        set(ska_rabbit_inc_dirs ${SKA_RABBIT_INCLUDE_DIR})
        unset(SKA_RABBIT_INCLUDE_DIR)
    endif()

    find_path(SKA_RABBIT_INCLUDE_DIR rabbit/Version.h
              PATHS ${ska_rabbit_inc_dirs}
              ${SKA_RABBIT_INSTALL_DIR}/include
              /usr/local/include
              /usr/include
    )

    set(ska_rabbit_names rabbit)
    foreach(lib ${ska_rabbit_names})
        find_library(ska_rabbit_library_${lib}
                     NAMES ${lib}
                     PATHS ${SKA_RABBIT_LIBRARY_DIRS}
                     ${SKA_RABBIT_INSTALL_DIR}
                     ${SKA_RABBIT_INSTALL_DIR}/lib
                     /usr/local/lib
                     /usr/lib
        )
        list(APPEND SKA_RABBIT_LIBRARIES ${ska_rabbit_library_${lib}})
    endforeach()

    include(FindPackageHandleCompat)

    # Set SKA_RABBIT_FOUND to TRUE if all listed variables are TRUE
    find_package_handle_standard_args(SKA_RABBIT DEFAULT_MSG SKA_RABBIT_LIBRARIES SKA_RABBIT_INCLUDE_DIR)

    if(NOT SKA_RABBIT_FOUND)
        set(SKA_RABBIT_LIBRARIES)
        set(SKA_RABBIT_INCLUDE_DIR)
        set(SKA_RABBIT_INCLUDE_DIRS)
    else()
        add_definitions(-DENABLE_SKA_RABBIT)
        list(APPEND SKA_RABBIT_INCLUDE_DIRS ${SKA_RABBIT_INCLUDE_DIR})
        message("Found rabbit; includes: ${SKA_RABBIT_INCLUDE_DIRS}; libs: ${SKA_RABBIT_LIBRARIES}")
    endif()

    mark_as_advanced(SKA_RABBIT_LIBRARIES SKA_RABBIT_INCLUDE_DIR SKA_RABBIT_INCLUDE_DIRS)

endif()
