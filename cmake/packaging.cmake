if(NOT SKA_CHEETAH_PACKAGING_GUARD_VAR)
    set(SKA_CHEETAH_PACKAGING_GUARD_VAR TRUE)
else()
    return()
endif()

# Packaging
set(CPACK_PACKAGE_VENDOR "The SKAO PSS Team")

# Get OS version information
execute_process(COMMAND cat /etc/os-release
                COMMAND grep PRETTY_NAME
                OUTPUT_VARIABLE OS_RELEASE_INFO
                ERROR_VARIABLE ERROR
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
if(NOT ${ERROR} EQUAL 0)
  message(FATAL_ERROR ${ERROR})
endif()

# Create substring for OS version
string(REGEX MATCH "\"[-a-zA-Z0-9_ \\.|$]*[a-zA-Z0-9_ \\.|$]\"" OS_INFO ${OS_RELEASE_INFO})
# Remove the "". This has to be done as a separate step as the previous step needed them as a unique part of the string to match on.
string(REGEX MATCH "[-a-zA-Z0-9_ \\.|$]*[a-zA-Z0-9_ \\.|$]" OS_VERSION ${OS_INFO})
# Add OS to description
string(CONCAT DESCRIPTION "Cheetah built using CMake on " ${OS_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DESCRIPTION})

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_BINARY_DEB "ON")
set(CPACK_BINARY_RPM "OFF")
set(CPACK_BINARY_STGZ "OFF")
set(CPACK_BINARY_TBZ2 "OFF")
set(CPACK_BINARY_TGZ "OFF")
set(CPACK_BINARY_TXZ "OFF")
set(CPACK_BINARY_TZ "OFF")
set(CPACK_BINARY_ZIP "OFF")
set(CPACK_SOURCE_DEB "ON")
set(CPACK_SOURCE_RPM "OFF")
set(CPACK_SOURCE_STGZ "OFF")
set(CPACK_SOURCE_TBZ2 "OFF")
set(CPACK_SOURCE_TGZ "OFF")
set(CPACK_SOURCE_TXZ "OFF")
set(CPACK_SOURCE_TZ "OFF")
set(CPACK_SOURCE_ZIP "OFF")
set(CPACK_SYSTEM_NAME ${OS_VERSION})
set(CPACK_TOPLEVEL_TAG ${OS_VERSION})

# Debian specifics
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-dev, libboost-program-options-dev, libboost-system-dev, libboost-filesystem-dev, cmake, libfftw3-dev, gcc, make")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER pss@skao.int)

# Build up the filename based on enabled flags
set(CPACK_PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME cheetah-${CPACK_PACKAGE_VERSION})
if(ENABLE_CUDA)
    string(CONCAT CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION} -cuda)
    string(CONCAT CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME} -cuda)
    # We are currently unable to set package depends on cuda due to the complexity of doing so
endif()
if(ENABLE_NASM)
    string(CONCAT CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION} -nasm)
    string(CONCAT CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME} -nasm)
    string(CONCAT CPACK_DEBIAN_PACKAGE_DEPENDS ${CPACK_DEBIAN_PACKAGE_DEPENDS} ", nasm")
endif()
if(ENABLE_INTEL_FPGA)
    string(CONCAT CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION} -intel_fpga)
    string(CONCAT CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME} -intel_fpga)
    # We are currently unable to set package depends on intel_fpga due to the complexity of doing so
endif()
string(CONCAT CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
string(CONCAT CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})

set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_NSIS_PACKAGE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${CPACK_PACKAGE_FILE_NAME})

include(CPack)