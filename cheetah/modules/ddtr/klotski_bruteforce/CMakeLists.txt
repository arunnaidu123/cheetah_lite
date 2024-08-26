set(MODULE_KLOTSKI_BRUTEFORCE_LIB_SRC_CPU
    src/Config.cpp
    src/ThreadedDedispersion.cpp
    src/kernels/nasm_integrate.asm
    src/kernels/nasm_klotski_bruteforce.asm
    src/kernels/nasm_downsample.asm
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

#TEST_UTILS()

add_subdirectory(test)
