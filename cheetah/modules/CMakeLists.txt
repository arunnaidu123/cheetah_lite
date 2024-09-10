subpackage(ddtr)
subpackage(rfim)
subpackage(spdt)
subpackage(sps_clustering)
subpackage(spsift)

set(MODULE_MODULES_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
    )

set(MODULE_MODULES_LIB_SRC_CPU
    ${LIB_SRC_CPU}
    PARENT_SCOPE
    )