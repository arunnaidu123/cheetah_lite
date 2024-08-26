subpackage(ddtr)
subpackage(rfim)
subpackage(sps)
subpackage(spsift)
subpackage(sps_clustering)

set(MODULE_MODULES_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
    )

set(MODULE_MODULES_LIB_SRC_CPU
    ${LIB_SRC_CPU}
    PARENT_SCOPE
    )