set(MODULE_UTILS_LIB_SRC_CPU
    src/Config.cpp
    src/ConvolvePlan.cpp
    src/System.cpp
    src/MultiThread.cpp
    src/TerminateException.cpp
    src/Version.cpp
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
