set(MODULE_SIGPROC_LIB_SRC_CPU
    src/Config.cpp
    src/SigProcFileStream.cpp
    src/SigProcHeader.cpp
    src/SigProcWriter.cpp
    src/WriterConfig.cpp
    PARENT_SCOPE
)

test_utils()
set(MODULE_SIGPROC_TEST_UTILS_SRC_CPU ${MODULE_SIGPROC_TEST_UTILS_SRC_CPU} PARENT_SCOPE)
add_subdirectory(test)
