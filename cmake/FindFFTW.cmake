#=======================================================================
# FindFFTW
# --------
#
# Finds the FFTW library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will define the following variables:
#
#     ``FFTW_FOUND``
#       TRUE if FFTW is found on the system.
#
#     ``FFTW_LIBRARIES``
#       Full paths to found FFTW libraries.
#
#     ``FFTW_INCLUDE_DIR``
#       Directory where FFTW includes were found.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables will be checked by the function:
#
#     ``FFTW_USE_STATIC_LIBS``
#       If TRUE, only static libraries are used.
#
#     ``FFTW_ROOT``
#       If set, the libraries are exclusively searched under this path.
#
#=======================================================================

if(NOT SKA_CHEETAH_FFTW_GUARD_VAR)
    set(SKA_CHEETAH_FFTW_GUARD_VAR TRUE)
else()
    return()
endif()

# If environment variable FFTWDIR is specified, it has same effect as FFTW_ROOT
if(NOT FFTW_ROOT AND ENV{FFTWDIR})
    set(FFTW_ROOT $ENV{FFTWDIR})
endif()

# Check if PkgConfig is available
find_package(PkgConfig)

# Determine location of FFTW from PkgConfig
if(PKG_CONFIG_FOUND AND NOT FFTW_ROOT)
    pkg_check_modules(PKG_FFTW "fftw3")
endif()

# Check whether to search static or dynamic libraries
set(cmake_find_library_suffixes_sav ${CMAKE_FIND_LIBRARY_SUFFIXES})
if(${FFTW_USE_STATIC_LIBS})
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

if(FFTW_ROOT)
    # Find libs
    find_library(FFTW_LIBRARY
                 NAMES "fftw3"
                 PATHS ${FFTW_ROOT}
                 PATH_SUFFIXES "lib" "lib64"
                 NO_DEFAULT_PATH
    )
    find_library(FFTWF_LIBRARY
                 NAMES "fftw3f"
                 PATHS ${FFTW_ROOT}
                 PATH_SUFFIXES "lib" "lib64"
                 NO_DEFAULT_PATH
    )
    find_library(FFTWL_LIBRARY
                 NAMES "fftw3l"
                 PATHS ${FFTW_ROOT}
                 PATH_SUFFIXES "lib" "lib64"
                 NO_DEFAULT_PATH
    )
    # Find includes
    find_path(FFTW_INCLUDE_DIR
              NAMES "fftw3.h"
              PATHS ${FFTW_ROOT}
              PATH_SUFFIXES "include"
              NO_DEFAULT_PATH
    )
else()
    # Find libs
    find_library(FFTW_LIBRARY
                 NAMES "fftw3"
                 PATHS ${PKG_FFTW_LIBRARY_DIRS}
                 ${LIB_INSTALL_DIR}
    )
    find_library(FFTWF_LIBRARY
                 NAMES "fftw3f"
                 PATHS ${PKG_FFTW_LIBRARY_DIRS}
                 ${LIB_INSTALL_DIR}
    )
    find_library(FFTWL_LIBRARY
                 NAMES "fftw3l"
                 PATHS ${PKG_FFTW_LIBRARY_DIRS}
                 ${LIB_INSTALL_DIR}
    )
    # Find includes
    find_path(FFTW_INCLUDE_DIR
              NAMES "fftw3.h"
              PATHS ${PKG_FFTW_INCLUDE_DIRS}
              ${INCLUDE_INSTALL_DIR}
    )
endif()

set(FFTW_LIBRARIES ${FFTW_LIBRARY} ${FFTWF_LIBRARY})

if(FFTWL_LIBRARY)
    list(APPEND FFTW_LIBRARIES ${FFTWL_LIBRARY})
endif()

set(CMAKE_FIND_LIBRARY_SUFFIXES ${cmake_find_library_suffixes_sav})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(FFTW DEFAULT_MSG FFTW_INCLUDE_DIR FFTW_LIBRARIES)

mark_as_advanced(FFTW_INCLUDE_DIR FFTW_LIBRARIES FFTW_LIBRARY FFTWF_LIBRARY FFTWL_LIBRARY)
