set(MODULE_CHANNEL_MASK_LIB_SRC_CPU
    src/Config.cpp
    src/ChannelRangeConfig.cpp
    src/FlaggedChannels.cpp
    PARENT_SCOPE
)

test_utils()
add_subdirectory(test)
