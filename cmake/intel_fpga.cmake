#================================================================================
# Find INTEL FPGA on PANDA
# This CMAKE part verifies if the INTEL-FPGA code was enabled in PANDA.
# If this feature has been enabled in PANDA, then it will activate the
# FDAS-FPGA module to be included in CHEETAH.
#================================================================================

# This variable would be available only if it was enabled on Panda.
option(ENABLE_INTEL_FPGA "Enable FDAS FPGA module in Cheetah." OFF)

if(NOT SKA_CHEETAH_INTEL_FPGA_GUARD_VAR)
    set(SKA_CHEETAH_INTEL_FPGA_GUARD_VAR TRUE)
else()
    return()
endif()

if(ENABLE_INTEL_FPGA)
    message(STATUS "Enabled FDAS FPGA module.")
    # Note that this line will be removed once the AT4-1506 is fixed.
    include_directories(${PANDA_INCLUDE_DIR}/panda/arch/intel_agilex_fpga)
endif(ENABLE_INTEL_FPGA)
