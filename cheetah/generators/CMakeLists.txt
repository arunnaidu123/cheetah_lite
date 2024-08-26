subpackage(pulse_profile)

set(MODULE_GENERATORS_LIB_SRC_CPU
    src/Config.cpp
    src/DispersedPulseConfig.cpp
    src/GeneratorAppConfig.cpp
    src/GaussianNoiseConfig.cpp
    src/SimplePulsarConfig.cpp
    src/BasebandGaussianNoiseConfig.cpp
    src/PhaseModelFactory.cpp
    src/PulsarInjectionConfig.cpp
    src/SimplePhaseModelConfig.cpp
    src/SimplePhaseModel.cpp
    src/Tempo2PhaseModel.cpp
    src/Tempo2PhaseModelConfig.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

add_executable(cheetah_generator src/generator_main.cpp)
install(TARGETS cheetah_generator DESTINATION ${BINARY_INSTALL_DIR})
target_link_libraries(cheetah_generator ${CHEETAH_LIBRARIES})

test_utils()
add_subdirectory(test)
