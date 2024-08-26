if(ENABLE_PSRDADA)
    set(MODULE_PSRDADA_LIB_SRC_CPU
        src/DadaClientBase.cpp
        src/DadaWriteClient.cpp
        src/DadaReadClient.cpp
        src/MultiLog.cpp
        src/RawBytes.cpp
        src/RawBytesReader.cpp
        src/RawBytesWriter.cpp
        src/SigProcDadaStream.cpp
        src/Config.cpp
        PARENT_SCOPE
    )
    test_utils()
    set(MODULE_PSRDADA_TEST_UTILS_SRC_CPU ${MODULE_PSRDADA_TEST_UTILS_SRC_CPU} PARENT_SCOPE)
    add_subdirectory(test)
endif()
