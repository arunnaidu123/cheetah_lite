#=======================================================================
# FindAstroaccelerate
# -------------------
#
# Find the native AstroAccelerate includes and library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
# This will set the following variables:
#
#     ``ASTROACCELERATE_FOUND``
#       TRUE if AstroAccelerate is found.
#
#     ``ASTROACCELERATE_INCLUDE_DIRS``
#       Directories where AstroAccelerate includes were found.
#
#     ``ASTROACCELERATE_LIBRARIES``
#       List of libraries when using AstroAccelerate.
#
#     ``ASTROACCELERATE_TEST_LIBRARIES``
#       List of AstroAccelerate test support libraries.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to explicitly specify the locations of AstroAccelerate components:
#
#     ``ASTROACCELERATE_INSTALL_DIR``
#       Where AstroAccelerate has been installed (lib/ and include/ directories included).
#
#     ``ASTROACCELERATE_LIBRARY_DIRS``
#       Explicitly define in which directories AstroAccelerate libraries may be found.
#
#     ``ASTROACCELERATE_INCLUDE_DIR``
#       Explicitly define in which directory AstroAccelerate includes may be found.
#
#=======================================================================

# Note there are no guards here; the following logic only runs if AstroAccelerate has not
# been found, so this file can be called multiple times if it is not found initially

if(ENABLE_CUDA)

    if(NOT ASTROACCELERATE_FOUND)

        if(ASTROACCELERATE_INCLUDE_DIR)
            set(astroaccelerate_inc_dirs ${ASTROACCELERATE_INCLUDE_DIR})
            unset(ASTROACCELERATE_INCLUDE_DIR)
        endif()

        find_path(ASTROACCELERATE_INCLUDE_DIR astro-accelerate/version.h
                  PATHS ${astroaccelerate_inc_dirs}
                  ${ASTROACCELERATE_INSTALL_DIR}/include
                  /usr/local/include
                  /usr/include
        )

        set(astroaccelerate_names astro-accelerate)
        foreach(lib ${astroaccelerate_names})
            find_library(astroaccelerate_library_${lib}
                         NAMES ${lib}
                         PATHS ${ASTROACCELERATE_LIBRARY_DIRS}
                         ${ASTROACCELERATE_INSTALL_DIR}
                         ${ASTROACCELERATE_INSTALL_DIR}/lib
                         /usr/local/lib
                         /usr/lib
            )
            list(APPEND ASTROACCELERATE_LIBRARIES ${astroaccelerate_library_${lib}})
        endforeach()

        include(FindPackageHandleCompat)

        # Set ASTROACCELERATE_FOUND to TRUE if all listed variables are TRUE
        find_package_handle_standard_args(ASTROACCELERATE DEFAULT_MSG ASTROACCELERATE_LIBRARIES ASTROACCELERATE_INCLUDE_DIR)

        if(NOT ASTROACCELERATE_FOUND)
            set(ASTROACCELERATE_LIBRARIES)
            set(ASTROACCELERATE_TEST_LIBRARIES)
            set(ASTROACCELERATE_INCLUDE_DIR)
            set(ASTROACCELERATE_INCLUDE_DIRS)
        else()
            # Add dependencies
            list(APPEND ASTROACCELERATE_LIBRARIES ${CUDA_curand_LIBRARY})
            list(APPEND ASTROACCELERATE_INCLUDE_DIRS ${ASTROACCELERATE_INCLUDE_DIR} "${ASTROACCELERATE_INCLUDE_DIR}/astro-accelerate")
            message("Found AstroAccelerate; includes: ${ASTROACCELERATE_INCLUDE_DIRS}; libs: ${ASTROACCELERATE_LIBRARIES}")
        endif()

        mark_as_advanced(ASTROACCELERATE_LIBRARIES ASTROACCELERATE_TEST_LIBRARIES ASTROACCELERATE_INCLUDE_DIR ASTROACCELERATE_INCLUDE_DIRS)

    endif()

endif()
