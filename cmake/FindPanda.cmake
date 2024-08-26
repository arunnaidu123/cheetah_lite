#=======================================================================
# FindPanda
# ---------
#
# Find the native PANDA includes and library.
#
# Components
# ^^^^^^^^^^
#
# cmake - Just the PANDA cmake components; e.g. set PANDA_SOURCE_DIR to point to the source.
#
# libs - The PANDA libraries and header files.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# cmake -
#
#     This will set the following variables:
#
#         ``PANDA_CMAKE_FOUND``
#           TRUE or FALSE
#
#         ``PANDA_CMAKE_MODULE_PATH``
#           Directory containing the PANDA exported cmake modules. This could be used as follows:
#               list(APPEND cmake_MODULE_PATH ${PANDA_CMAKE_MODULE_PATH})
#
# libs -
#
#     This will set the following variables:
#
#         ``PANDA_LIBRARIES``
#           List of libraries when using PANDA.
#
#         ``PANDA_TEST_LIBRARIES``
#           List of PANDA test support libraries.
#
#         ``PANDA_INCLUDE_DIR``
#           Directory where PANDA includes were found.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
#  The following variables can be set to explicitly specify the locations of PANDA components in order of precedence:
#
#      ``PANDA_LIBRARY_DIRS``
#        Explicitly define in which directories PANDA libraries may be found.
#
#      ``PANDA_INCLUDE_DIR``
#        Explicitly define in which directory PANDA includes may be found.
#
#      ``PANDA_MODULES_DIR``
#        Explicitly define in which directory PANDA modules may be found.
#
#      ``PANDA_SOURCE_DIR``
#        Top of the PANDA source-files directory.
#
#      ``PANDA_INSTALL_DIR``
#        Where PANDA framwork has been installed (lib/ and include/ directories included).
#
#=======================================================================

# Note there are no guards here; the following logic only runs if PANDA has not
# been found, so this file can be called multiple times if it is not found initially

if(NOT PANDA_FOUND)

    include(FindPackageHandleCompat)

    # If no component has been explicitly requested, default to everything
    if(NOT PANDA_FIND_COMPONENTS)
        set(PANDA_FIND_COMPONENTS libs cmake)
    endif()

    # Spin through all components
    foreach(component ${PANDA_FIND_COMPONENTS})

        string(TOLOWER ${component} component)
        if(${component} MATCHES "cmake")
            # Exported cmake modules
            if(PANDA_MODULES_DIR)
                set(panda_mod_dirs ${PANDA_MODULES_DIR})
                unset(PANDA_MODULES_DIR)
            endif()
            find_path(PANDA_MODULES_DIR panda/cmake/git_version.cmake
                      "${panda_mod_dirs}"
                      "${PANDA_SOURCE_DIR}"
                      "${PANDA_INSTALL_DIR}/share"
                      /usr/local/share
                      /usr/share
            )
            message("Found PANDA_MODULES_DIR: ${PANDA_MODULES_DIR}")

            # Set PANDA_MODULES_FOUND to TRUE if all listed variables are TRUE
            find_package_handle_standard_args(PANDA_MODULES DEFAULT_MSG PANDA_MODULES_DIR)

            if(PANDA_MODULES_FOUND)
                set(PANDA_CMAKE_MODULE_PATH "${PANDA_MODULES_DIR}/panda/cmake")
                set(PANDA_CMAKE_FOUND TRUE) # For dependency checking
            else()
                set(PANDA_CMAKE_FOUND FALSE)
            endif()

            mark_as_advanced(PANDA_MODULES_DIR)

        endif()

        if(${component} MATCHES "libs")
            # Include files
            if(PANDA_INCLUDE_DIR)
                set(panda_inc_dirs ${PANDA_INCLUDE_DIR})
                unset(PANDA_INCLUDE_DIR)
            endif()
            find_path(PANDA_INCLUDE_DIR panda/Version.h
                      PATHS ${panda_inc_dirs}
                      ${PANDA_SOURCE_DIR}
                      ${PANDA_INSTALL_DIR}/include
                      /usr/local/include
                      /usr/include
            )
            message("Found PANDA_INCLUDE_DIR: ${PANDA_INCLUDE_DIR}")

            # Libraries
            set(panda_names panda)
            foreach(lib ${panda_names})
                find_library(panda_library_${lib}
                             NAMES ${lib}
                             PATHS ${PANDA_LIBRARY_DIRS}
                             ${PANDA_INSTALL_DIR}
                             ${PANDA_INSTALL_DIR}/lib
                             /usr/local/lib
                             /usr/lib
                )
                list(APPEND PANDA_LIBRARIES ${panda_library_${lib}})
            endforeach()

            # Test libraries
            set(panda_test_names panda_testutils)
            foreach(lib ${panda_test_names})
                find_library(panda_library_${lib}
                             NAMES ${lib}
                             PATHS ${PANDA_LIBRARY_DIRS}
                             ${PANDA_LIBRARY_DIRS}/test
                             ${PANDA_INSTALL_DIR}/lib
                             /usr/local/lib
                             /usr/lib
                )
                list(APPEND PANDA_TEST_LIBRARIES ${panda_library_${lib}})
            endforeach()

            # Set PANDA_LIBS_FOUND to TRUE if all listed variables are TRUE
            find_package_handle_standard_args(PANDA_LIBS DEFAULT_MSG PANDA_LIBRARIES PANDA_INCLUDE_DIR)

            set(PANDA_libs_FOUND ${PANDA_LIBS_FOUND}) # Required for component checking
            if(NOT PANDA_LIBS_FOUND)
                set(PANDA_LIBRARIES)
                set(PANDA_TEST_LIBRARIES)
            endif()

            mark_as_advanced(PANDA_LIBRARIES PANDA_TEST_LIBRARIES PANDA_LIBRARY_DIRS PANDA_INCLUDE_DIR)

        endif()

    endforeach()

    # This call to find_package_handle_standard_args() is just to correctly handle the components (cmake, libs, etc).
    # Using HANDLE_COMPONENTS requires using REQUIRED_VARS as well, and REQUIRED_VARS does require a variable to be set.
    # Since the components are optional, pass it a dummy variable. All other required variables are dealt with above.
    set(dummy_required_var 1)
    find_package_handle_standard_args(PANDA REQUIRED_VARS dummy_required_var HANDLE_COMPONENTS)
    unset(dummy_required_var)

    if(PANDA_FOUND)
        message("Found PANDA")
    else()
        message("Failed to find PANDA")
    endif()

endif()
