# Wrapper around FindPackageHandleStandardArgs required for message printing
# in find_package() macros to fix compatibility with CMake before version 2.5

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.5)
    include(FindPackageHandleStandardArgs)
else()
    #========================================================
    # find_package_handle_standard_args
    #
    # brief: define a simple version of find_package_handle_standard_args(),
    #        which is absent from earlier versions of CMake
    #
    # usage: find_package_handle_standard_args(package)
    #========================================================
    macro(find_package_handle_standard_args package)
        if(${package}_LIBRARY AND ${package}_INCLUDE_DIR)
            set(${package}_FOUND TRUE)
        endif()
    endmacro()
endif()
