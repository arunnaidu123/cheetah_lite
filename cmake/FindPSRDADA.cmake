#=======================================================================
# FindPSRDADA
# -----------
#
# Find the PSRDADA package.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will set the following variables:
#
#     ``PSRDADA_FOUND``
#       TRUE if PSRDADA is found.
#
#     ``PSRDADA_LIBRARIES``
#       List of libraries when using PSRDADA.
#
#     ``PSRDADA_INCLUDE_DIRS``
#       Directories where PSRDADA includes were found.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to explicitly specify the locations of PSRDADA components:
#
#     ``PSRDADA_INSTALL_DIR``
#       Where PSRDADA has been installed (lib/ and include/ directories included).
#
#     ``PSRDADA_LIBRARY_DIRS``
#       Explicitly define in which directories PSRDADA libraries may be found.
#
#     ``PSRDADA_INCLUDE_DIR``
#       Explicitly define in which directory PSRDADA includes may be found.
#
#=======================================================================
option(ENABLE_PSRDADA "Enable PSRDADA capabilties. Requires PSRDADA library to be installed" OFF)

if(NOT SKA_CHEETAH_PSRDADA_GUARD_VAR)
    set(SKA_CHEETAH_PSRDADA_GUARD_VAR TRUE)
else()
    return()
endif()

if(ENABLE_PSRDADA)

    if(PSRDADA_INCLUDE_DIR)
        set(psrdada_inc_dirs ${PSRDADA_INCLUDE_DIR})
        unset(PSRDADA_INCLUDE_DIR)
    endif()

    find_path(PSRDADA_INCLUDE_DIR dada_def.h
              PATHS ${psrdada_inc_dirs}
              ${PSRDADA_INSTALL_DIR}/include
              /usr/local/include
              /usr/include
    )

    set(psrdada_names psrdada)
    foreach(lib ${psrdada_names})
        find_library(psrdada_library_${lib}
                     NAMES ${lib}
                     PATHS ${PSRDADA_LIBRARY_DIRS}
                     ${PSRDADA_INSTALL_DIR}
                     ${PSRDADA_INSTALL_DIR}/lib
                     /usr/local/lib
                     /usr/lib
        )
        list(APPEND PSRDADA_LIBRARIES ${psrdada_library_${lib}})
    endforeach()

    include(FindPackageHandleCompat)

    # Set PSRDADA_FOUND to TRUE if all listed variables are TRUE
    find_package_handle_standard_args(PSRDADA DEFAULT_MSG PSRDADA_LIBRARIES PSRDADA_INCLUDE_DIR)

    if(NOT PSRDADA_FOUND)
        set(PSRDADA_LIBRARIES)
        set(PSRDADA_INCLUDE_DIR)
        set(PSRDADA_INCLUDE_DIRS)
    else()
        set(CMAKE_CXX_FLAGS "-DENABLE_PSRDADA ${CMAKE_CXX_FLAGS}")
        list(APPEND PSRDADA_INCLUDE_DIRS ${PSRDADA_INCLUDE_DIR})
        message("Found PSRDADA; includes: ${PSRDADA_INCLUDE_DIRS}; libs: ${PSRDADA_LIBRARIES}")
    endif()

    mark_as_advanced(PSRDADA_LIBRARIES PSRDADA_INCLUDE_DIR PSRDADA_INCLUDE_DIRS)

endif()
