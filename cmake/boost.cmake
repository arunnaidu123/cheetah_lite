if(NOT SKA_CHEETAH_BOOST_GUARD_VAR)
    set(SKA_CHEETAH_BOOST_GUARD_VAR TRUE)
else()
    return()
endif()

if(PANDA_CMAKE_MODULE_PATH)
    include("${PANDA_CMAKE_MODULE_PATH}/boost.cmake")
else()
    message(FATAL_ERROR "Dependency Error: include boost.cmake after panda has been found")
endif()

# Add extra boost components
find_package(Boost COMPONENTS filesystem system program_options REQUIRED)

set(BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
set(BOOST_LIBRARIES ${Boost_LIBRARIES})
set(BOOST_MAJOR_VERSION ${Boost_MAJOR_VERSION})
set(BOOST_MINOR_VERSION ${Boost_MINOR_VERSION})

if(BOOST_MAJOR_VERSION EQUAL 1 AND BOOST_MINOR_VERSION LESS 58)
    # Pick up boost::endian from the third-party copy
    list(APPEND BOOST_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdparty/boost")
endif()

if(BOOST_MAJOR_VERSION EQUAL 1 AND BOOST_MINOR_VERSION LESS 54)
    # Pick up boost::geometry from version 1.55 of the third-party copy
    list(INSERT BOOST_INCLUDE_DIRS 0 "${PROJECT_SOURCE_DIR}/thirdparty/boost/1.55")
endif()

if(BOOST_MAJOR_VERSION EQUAL 1 AND BOOST_MINOR_VERSION LESS 56)
    # Pick up boost::align from version 1.56 of the third-party copy
    list(APPEND BOOST_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdparty/boost/1.56")
endif()
