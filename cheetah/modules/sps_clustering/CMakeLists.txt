set(MODULE_SPS_CLUSTERING_LIB_SRC_CPU
    src/Config.cpp
    src/Fof.cpp
    src/SpsClustering.cpp
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
