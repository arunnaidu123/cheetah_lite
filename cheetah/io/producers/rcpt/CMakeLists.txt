set(MODULE_RCPT_LIB_SRC_CPU
    src/Config.cpp
    src/Sample.cpp
    src/SkaPacketStreamFactory.cpp
    src/SkaSelector.cpp
    src/SkaUdpConfig.cpp
    PARENT_SCOPE
)

#test_utils()
add_subdirectory(test)
