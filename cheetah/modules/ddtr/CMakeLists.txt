subpackage(cpu)
subpackage(klotski)
subpackage(klotski_bruteforce)
set(MODULE_DDTR_LIB_SRC_CPU
    src/Config.cpp
    src/DedispersionConfig.cpp
    src/DedispersionTrialPlan.cpp
    src/RfiExcisionConfig.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_DDTR_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

#test_utils()
#add_subdirectory(test)
