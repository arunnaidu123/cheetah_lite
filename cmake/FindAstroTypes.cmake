#=======================================================================
# FindAstroTypes
# --------------
#
# Find the native astrotypes includes.
#
# Components
# ^^^^^^^^^^
#
# cmake - Just the astrotypes cmake components; e.g. set ASTROTYPES_SOURCE_DIR to point to the source.
#
# headers - The astrotypes header files.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# cmake -
#
#     This will set the following variables:
#
#         ``ASTROTYPES_CMAKE_FOUND``
#           TRUE or FALSE
#
#         ``ASTROTYPES_CMAKE_MODULE_PATH``
#           Directory containing the astrotypes exported cmake modules. This could be used as follows:
#               list(APPEND cmake_MODULE_PATH ${ASTROTYPES_CMAKE_MODULE_PATH})
#
# headers -
#
#     This will set the following variable:
#
#         ``ASTROTYPES_INCLUDE_DIR``
#           Directory where astrotypes includes were found.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to explicitly specify the locations of PANDA components in order of precedence:
#
#     ``ASTROTYPES_INCLUDE_DIR``
#       Explicitly define in which directory astrotypes includes may be found.
#
#     ``ASTROTYPES_MODULES_DIR``
#       Explicitly define in which directory astrotypes modules may be found.
#
#     ``ASTROTYPES_SOURCE_DIR``
#       Top of the astrotypes source-files directory.
#
#     ``ASTROTYPES_INSTALL_DIR``
#       Where astrotypes has been installed (lib/ and include/ directories included).
#
#=======================================================================

option(USE_THIRDPARTY_ASTROTYPES "Force the use of the version of astrotypes in the thirdparty directory, even if a system install is present" FALSE)

if(NOT SKA_CHEETAH_ASTROTYPES_GUARD_VAR)
    set(SKA_CHEETAH_ASTROTYPES_GUARD_VAR TRUE)
else()
    return()
endif()

# Process user input files and set defaults
# If no component explicitly requested, then default to headers
if(NOT ASTROTYPES_FIND_COMPONENTS)
    set(ASTROTYPES_FIND_COMPONENTS headers)
endif()

#========================================================
# search_astrotypes
#
# brief: search for installed astrotypes headers
#
# usage: search_astrotypes()
#========================================================
function(search_astrotypes)

    # Include files
    if(ASTROTYPES_INCLUDE_DIR)
        set(astrotypes_inc_dirs ${ASTROTYPES_INCLUDE_DIR})
        unset(ASTROTYPES_INCLUDE_DIR CACHE)
    endif()

    # CMake files
    if(ASTROTYPES_MODULES_DIR)
        set(astrotypes_mod_dirs ${ASTROTYPES_MODULES_DIR})
        unset(ASTROTYPES_MODULES_DIR)
    endif()

    foreach(component ${ASTROTYPES_FIND_COMPONENTS})

        string(TOLOWER ${component} component)

        if(${component} MATCHES "headers")

            # Check for header files and set ASTROTYPES_INCLUDE_DIR
            find_path(ASTROTYPES_INCLUDE_DIR pss/astrotypes/version.h
                      PATHS ${astrotypes_inc_dirs}
                      ${ASTROTYPES_INSTALL_DIR}/include
                      /usr/local/include
                      /usr/include
            )

            # Verify ASTROTYPES_INCLUDE_DIR is set and valid
            # If so, ASTROTYPES_HEADERS_FOUND is set to TRUE
            find_package_handle_standard_args(ASTROTYPES_HEADERS DEFAULT_MSG ASTROTYPES_INCLUDE_DIR)

            if(ASTROTYPES_HEADERS_FOUND)
                set(ASTROTYPES_HEADERS_FOUND ${ASTROTYPES_HEADERS_FOUND} PARENT_SCOPE)
                set(ASTROTYPES_INCLUDE_DIR ${ASTROTYPES_INCLUDE_DIR} PARENT_SCOPE)
            endif()

        endif()

        if(${component} MATCHES "cmake")
            find_path(ASTROTYPES_MODULES_DIR astrotypes/cmake/git_version.cmake
                      ${astrotypes_mod_dirs}
                      ${ASTROTYPES_SOURCE_DIR}
                      ${ASTROTYPES_INSTALL_DIR}/share
                      /usr/local/share
                      /usr/share
            )
            # Verify ASTROTYPES_MODULES_DIR is set and valid
            # If so, ASTROTYPES_MODULES_FOUND is set to TRUE
            find_package_handle_standard_args(ASTROTYPES_MODULES DEFAULT_MSG ASTROTYPES_MODULES_DIR)

            if(ASTROTYPES_MODULES_FOUND)
                set(ASTROTYPES_CMAKE_MODULE_PATH "${ASTROTYPES_MODULES_DIR}/astrotypes/cmake" PARENT_SCOPE)
                set(ASTROTYPES_CMAKE_FOUND TRUE PARENT_SCOPE) # For dependency checking
            else()
                set(ASTROTYPES_CMAKE_FOUND FALSE PARENT_SCOPE)
            endif()
        endif()

    endforeach()

    # Verify ASTROTYPES_HEADERS_FOUND has been set to TRUE.
    # Since astrotypes is header-only, this is
    # sufficient to set ASTROTYPES_FOUND to TRUE.
    find_package_handle_standard_args(ASTROTYPES REQUIRED_VARS ASTROTYPES_HEADERS_FOUND)

    set(ASTROTYPES_FOUND ${ASTROTYPES_FOUND} PARENT_SCOPE)

endfunction(search_astrotypes)

include(FindPackageHandleCompat)

# Check if the user wants to use the included third-party
# astrotypes, regardless of if they have a existing install
if(NOT USE_THIRDPARTY_ASTROTYPES)
    message(STATUS "Checking for an installed astrotypes library...")
    search_astrotypes()
    if(ASTROTYPES_FOUND)
        message(STATUS "astrotypes found in ${ASTROTYPES_INCLUDE_DIR}")
    else() # If no astrotypes install is found, use the included third-party astrotypes
        message(STATUS "No astrotypes install found, defaulting to thirdparty astrotypes...")
        set(USE_THIRDPARTY_ASTROTYPES TRUE)
    endif()
endif()

if(USE_THIRDPARTY_ASTROTYPES)
    message(STATUS "Using thirdparty astrotypes; building astrotypes ...")
    set(ASTROTYPES_BINARY_DIR "${THIRDPARTY_BINARY_DIR}/astrotypes/src/cpp")
    set(ASTROTYPES_SOURCE_DIR "${THIRDPARTY_DIR}/astrotypes/src/cpp")
    add_thirdparty_subdirectory("pss/astrotypes" "${ASTROTYPES_SOURCE_DIR}")
    set(ASTROTYPES_INCLUDE_DIR "${ASTROTYPES_SOURCE_DIR}" "${ASTROTYPES_BINARY_DIR}")
    set(ASTROTYPES_CMAKE_FOUND TRUE) # For dependency checking
    # Check all required astrotypes components are set.
    # If present, this will return ASTROTYPES_FOUND.
    find_package_handle_standard_args(ASTROTYPES REQUIRED_VARS ASTROTYPES_BINARY_DIR ASTROTYPES_SOURCE_DIR ASTROTYPES_INCLUDE_DIR HANDLE_COMPONENTS)
endif()

mark_as_advanced(ASTROTYPES_INCLUDE_DIR ASTROTYPES_MODULES_DIR ASTROTYPES_SOURCE_DIR ASTROTYPES_INSTALL_DIR)
