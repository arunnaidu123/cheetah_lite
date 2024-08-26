set(MODULE_SPSIFT_LIB_SRC_CPU
    src/Config.cpp
    src/SpSift.cpp
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
