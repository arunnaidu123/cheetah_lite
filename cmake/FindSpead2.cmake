#=======================================================================
# FindSpead2
# ----------
#
# Find the spead2 includes and library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will set the following variables:
#
#     ``SPEAD_FOUND``
#       TRUE if spead2 is found.
#
#     ``SPEAD_LIBRARIES``
#       List of libraries when using spead2.
#
#     ``SPEAD_INCLUDE_DIR``
#       Directory where spead2 includes were found.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to explicitly specify the locations of spead2 components in order of precedence:
#
#     ``SPEAD_LIBRARY_DIRS``
#       Explicitly define in which directory spead2 libraries may be found.
#
#     ``SPEAD_INCLUDE_DIR``
#       Explicitly define in which directory spead2 includes may be found.
#
#     ``SPEAD_INSTALL_DIR``
#       Where spead2 framework has been installed (lib/ and include/ directories included).
#
#=======================================================================
option(ENABLE_SPEAD2 "Enable spead2" OFF)

if(NOT SKA_CHEETAH_SPEAD2_GUARD_VAR)
    set(SKA_CHEETAH_SPEAD2_GUARD_VAR TRUE)
else()
    return()
endif()

if(ENABLE_SPEAD2)

    include(FindPackageHandleCompat)

    # Include files
    if(SPEAD_INCLUDE_DIR)
        set(spead_inc_dir ${SPEAD_INCLUDE_DIR})
        unset(SPEAD_INCLUDE_DIR)
    endif()

    find_path(SPEAD_INCLUDE_DIR spead2/send_heap.h
              PATHS ${spead_inc_dir}
              ${SPEAD_INSTALL_DIR}/include
              /usr/local/include
              /usr/include
    )

    # Libraries
    set(spead_names spead2)
    foreach(lib ${spead_names})
        find_library(spead_library_${lib}
                     NAMES ${lib}
                     PATHS ${SPEAD_LIBRARY_DIRS}
                     ${SPEAD_INSTALL_DIR}
                     ${SPEAD_INSTALL_DIR}/lib
                     /usr/local/lib
                     /usr/lib
        )
        list(APPEND SPEAD_LIBRARIES ${spead_library_${lib}})
    endforeach()

    # Set SPEAD_FOUND to TRUE if all listed variables are TRUE
    find_package_handle_standard_args(SPEAD DEFAULT_MSG SPEAD_LIBRARIES SPEAD_INCLUDE_DIR)

    mark_as_advanced(SPEAD_LIBRARIES SPEAD_INCLUDE_DIR)

    if(SPEAD_FOUND)
        message("Found spead2 library:")
        message("   SPEAD INCLUDE_DIR: ${SPEAD_INCLUDE_DIR}")
        message("   SPEAD LIBRARIES: ${SPEAD_LIBRARIES}")
        SET(CMAKE_CXX_FLAGS "-DENABLE_SPEAD ${CMAKE_CXX_FLAGS}")
    else()
        set(SPEAD_LIBRARIES)
        message("spead2 library not found. Please specify one or more of SPEAD_INCLUDE_DIR, SPEAD_LIBRARY_DIRS, SPEAD_INSTALL_DIR")
    endif()

endif()
