set(MODULE_EMULATOR_LIB_SRC_CPU
    src/Config.cpp
    src/EmulatorAppConfig.cpp
    PARENT_SCOPE
)

add_executable(cheetah_emulator src/emulator_main.cpp)
install(TARGETS cheetah_emulator DESTINATION ${BINARY_INSTALL_DIR})
target_link_libraries(cheetah_emulator ${CHEETAH_LIBRARIES})

test_utils()
add_subdirectory(test)
