if(SPEAD_FOUND)
    set(spead_lib_src_cpu
        src/SpCclSpeadReaderConfig.cpp
        src/SpCclSpeadStreamer.cpp
        src/SpCclSpeadStreamerConfig.cpp
        src/SpeadLoggingAdapter.cpp
    )
endif()

set(MODULE_EXPORTERS_LIB_SRC_CPU
    src/DataExportConfig.cpp
    src/DataExportStreamConfig.cpp
    src/ExporterType.cpp
    src/FileStreamerConfig.cpp
    src/OcldFileStreamerConfig.cpp
    src/SclCandidateDataStreamer.cpp
    src/SclCandidateDataStreamerConfig.cpp
    src/SclCandidateDataStreamerTraits.cpp
    src/SclFileStreamer.cpp
    src/SclFileStreamerConfig.cpp
    src/SclFileStreamerTraits.cpp
    src/SpCclFileStreamerConfig.cpp
    src/SpCandidateDataStreamerConfig.cpp
    ${spead_lib_src_cpu}
    #src/SpCclSigProcConfig.cpp
    PARENT_SCOPE
)

#test_utils()
#add_subdirectory(test)
