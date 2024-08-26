set(MODULE_RCPT_LOW_LIB_SRC_CPU
    src/Config.cpp
    src/PacketGeneratorConfig.cpp
    src/UdpStreamFrequencyTime.cpp
    src/UdpStreamTimeFrequency.cpp
    PARENT_SCOPE
)

#test_utils()
add_subdirectory(test)
