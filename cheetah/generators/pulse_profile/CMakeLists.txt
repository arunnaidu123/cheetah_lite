# Add profile-specific CMake macros
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/profile_tools.cmake)

add_profile("B0011+47")
profile_init()

set(MODULE_PULSE_PROFILE_LIB_SRC_CPU
    src/AsciiProfileFile.cpp
    src/ProfileManager.cpp
    src/PulsarProfile.cpp
    src/PulsarProfileId.cpp
    src/PulsarProfileConfig.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
