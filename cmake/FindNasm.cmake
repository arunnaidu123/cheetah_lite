#=======================================================================
# FindNasm
# --------
#
# Find and set up the NASM assembler for NASM assembly
# code in files with .asm or .nasm extensions
#
# Further documentation can be found in the CMake NASM description file:
#
#     e.g. /usr/share/cmake-<version_number>/Modules/CMakeDetermineASM_NASMCompiler.cmake
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to specify the location of a component:
#
#     ``CMAKE_ASM_NASM_COMPILER``
#       The full path to the NASM executable. If not set, CMake will try to determine this automatically.
#
#     ``CMAKE_ASM_NASM_OBJECT_FORMAT``
#       If not set, CMake will attempt to compile for the current system.
#
#=======================================================================
option(ENABLE_NASM "Enable the NASM assembler for optimisations." OFF)

if(ENABLE_NASM)
    # The CMake module for NASM uses CMAKE_C_SIZEOF_DATA_PTR
    # which is not set for a C++ project, so we set it manually
    if(NOT CMAKE_C_SIZEOF_DATA_PTR)
        set(CMAKE_C_SIZEOF_DATA_PTR ${CMAKE_CXX_SIZEOF_DATA_PTR})
    endif()

    # Use CMake to determine the location of the NASM
    # compiler and set up rules for .nasm and .asm files
    enable_language(ASM_NASM)
    if(APPLE)
        set(CMAKE_ASM_NASM_FLAGS "--prefix _")
    endif()
endif()
