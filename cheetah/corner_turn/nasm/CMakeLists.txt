set(MODULE_NASM_LIB_SRC_CPU
    src/nasm_cornerturn_char.asm
    src/nasm_cornerturn_short.asm
    src/nasm_cornerturn_char_short.asm
    PARENT_SCOPE
)

add_subdirectory(test)
