subpackage(cuda)
subpackage(altera)

set(MODULE_DATA_LIB_SRC_CPU
    src/Birdie.cpp
    src/CandidateWindow.cpp
    src/CandidateWindowConfig.cpp
    src/Ccl.cpp
    src/DataSequence2D_bool.cpp
    src/DataSrcConfig.cpp
    src/DmTrialsMetadata.cpp
    src/Scl.cpp
    src/TimeFrequencyMetadata.cpp
    ${LIB_SRC_CPU}
    PARENT_SCOPE
)

set(MODULE_DATA_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
