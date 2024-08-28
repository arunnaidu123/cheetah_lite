subpackage(ddtr)
subpackage(rfim)
subpackage(spdt)

set(MODULE_MODULES_LIB_SRC_CUDA
    ${LIB_SRC_CUDA}
    PARENT_SCOPE
    )

set(MODULE_MODULES_LIB_SRC_CPU
    ${LIB_SRC_CPU}
    PARENT_SCOPE
    )