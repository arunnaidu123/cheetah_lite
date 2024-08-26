set(MODULE_RCPT_MID_LIB_SRC_CPU
    src/Config.cpp
    src/UdpStreamFrequencyTime.cpp
    src/UdpStreamTimeFrequency.cpp
    src/PacketGeneratorConfig.cpp
    PARENT_SCOPE
)

#test_utils()
add_subdirectory(test)
