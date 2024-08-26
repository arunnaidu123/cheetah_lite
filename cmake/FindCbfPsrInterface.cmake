#=======================================================================
# FindCbfPsrInterface
# -------------------
#
# Find the CBF Low packet includes.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will set the following variables:
#
#     ``CBF_PSR_INTERFACE_FOUND``
#       TRUE or FALSE
#
#     ``CBF_PSR_INTERFACE_INCLUDE_DIR``
#       Directory where CBF Low includes were found.
#
# Location Variables
# ^^^^^^^^^^^^^^^^^^
#
# The following variables can be set to explicitly specify the locations of CBF Low components:
#
#     ``CBF_PSR_INTERFACE_INCLUDE_DIR``
#       Explicitly define in which directory CBF Low includes may be found.
#
#=======================================================================

if(NOT SKA_CHEETAH_CBF_PSR_INTERFACE_GUARD_VAR)
    set(SKA_CHEETAH_CBF_PSR_INTERFACE_GUARD_VAR TRUE)
else()
    return()
endif()

if(NOT CBF_PSR_INTERFACE_FOUND)

    include(FindPackageHandleCompat)

    find_path(CBF_PSR_INTERFACE_INCLUDE_DIR "ska/cbf_psr_interface/CbfPsrHeader.h"
              PATHS ${THIRDPARTY_DIR}/cbf-psr-interface/src/
    )

    if(CBF_PSR_INTERFACE_INCLUDE_DIR)
        message("Found CBF_PSR_INTERFACE_INCLUDE_DIR: ${CBF_PSR_INTERFACE_INCLUDE_DIR}")
        set(CBF_PSR_INTERFACE_FOUND TRUE)
    endif()

endif()

find_package_handle_standard_args(LowPacketHeaders FOUND_VAR LowPacketHeaders_FOUND REQUIRED_VARS CBF_PSR_INTERFACE_INCLUDE_DIR)
