# googletest build - must be built with the same compiler flags
cmake_minimum_required(VERSION 3.14)
include(FetchContent)

set(GTEST_VERSION "release-1.12.1")
#set the destination directory for the download of googletest
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/googletest")

find_package(GTest)
if(GTest_FOUND)
  message("GTest found")

else()
  message("GTest not found. Fetching Googletest version ${GTEST_VERSION}")
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        ${GTEST_VERSION}
  )
  FetchContent_MakeAvailable(googletest)
endif()

if(NOT SKA_CHEETAH_GTEST_GUARD_VAR)
    set(SKA_CHEETAH_GTEST_GUARD_VAR TRUE)
else()
    return()
endif()

if(NOT GTEST_INCLUDE_DIR)
    if(APPLE)
        add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=1)
    endif()

    set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googletest-src/googletest/include)
    set(GMOCK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googletest-src/googlemock/include)
    set(GTEST_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(GTEST_LIBRARIES gtest_main gtest)
endif()
