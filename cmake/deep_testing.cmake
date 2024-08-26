option(DEEP_TESTING "Enable extended testing requiring external data sets not available in this repository." OFF)

if(NOT SKA_CHEETAH_DEEP_TESTING_GUARD_VAR)
    set(SKA_CHEETAH_DEEP_TESTING_GUARD_VAR TRUE)
else()
    return()
endif()

if(DEEP_TESTING)
    add_definitions(-DDEEP_TESTING)
    message("DEEP_TESTING activated")
endif()
