#===============================================================================
# These functions/macros generate the boilerplate for the ProfileManagerInitialiser class
#
# Add profiles with the
#
#     add_profile(profile_name)
#
# macro for files in the profile_data directory, or the
#
#     add_profile_from_file(profile_name data_file_name)
#
# for files elsewhere.
#
# When all profiles are added, call the profile_init() macro
#
# @example
# add_profile(profile_name_1)
# add_profile(profile_name_2)
# profile_init()
# @endexample
#===============================================================================

#===============================================================================
# add_profile_from_file
#
# brief: Generate a C++ PulsarProfile class from a data file
#
# usage: add_profile_from_file(profile_name data_file_name)
#===============================================================================
macro(add_profile_from_file profile file_name)
    set(unmodified_profile_name ${profile})
    string(REGEX REPLACE "[+]" "_plus_" profile_name ${profile})
    set(class_name "Profile_${profile_name}")
    set(profile_header_file "${class_name}.h")
    set(profile_cpp_file "${class_name}.cpp")
    file(READ ${file_name} raw_profile_data)
    string(REGEX REPLACE "\n" "," profile_data "${raw_profile_data}")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/Profile.h.in ${CMAKE_CURRENT_BINARY_DIR}/${profile_header_file})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/Profile.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/${profile_cpp_file})
    list(APPEND LIB_SRC_CPU ${profile_cpp_file})
    list(APPEND profile_includes ${profile_cpp_file})
    list(APPEND profile_classes ${class_name})
endmacro(add_profile_from_file)

#===============================================================================
# add_profile
#
# brief: Generate a C++ PulsarProfile class given a profile name
#
# usage: add_profile(profile_name)
#===============================================================================
macro(add_profile profile_name)
    add_profile_from_file(${profile_name} "profile_data/${profile_name}.txt")
endmacro(add_profile)

#===============================================================================
# profile_header
#
# brief: Create a header file and a cpp file based on a generated PulsarProfile
#
# usage: profile_header(header_file cpp_file)
#===============================================================================
function(profile_header header_file initialiser_file)

    # Generate an include file for initialising profiles
    set(header_file_gen "${header_file}.gen")
    file(WRITE ${header_file_gen} "// Generated file - do not edit\n")
    foreach(include_file ${profile_includes})
        file(WRITE ${header_file_gen} "#include \"${include_file}\"\n")
    endforeach()
    configure_file(${header_file_gen} ${header_file} COPYONLY)

    # Create a cpp file for initialising profiles
    set(initialiser_file_gen "${initialiser_file}.gen")
    foreach(class ${profile_classes})
        file(WRITE ${initialiser_file_gen} "        ${class}::add(manager);\n")
    endforeach()
    configure_file(${initialiser_file_gen} ${initialiser_file} COPYONLY)

endfunction(profile_header)

#===============================================================================
# profile_init
#
# brief: Initialise all generated profiles in the build system
#
# usage: profile_init()
#===============================================================================
macro(profile_init)
    set(profile_initialiser_cpp "ProfileManagerInitialiser.cpp")
    set(profile_include_file "${CMAKE_CURRENT_BINARY_DIR}/AddProfileInitilisationHeader.h")
    set(profile_init_file "${CMAKE_CURRENT_BINARY_DIR}/AddProfileInitialiser.cpp")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/ProfileManagerInitialiser.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/${profile_initialiser_cpp}")
    list(APPEND LIB_SRC_CPU ${profile_initialiser_cpp})
    profile_header(${profile_include_file} ${profile_init_file})
endmacro(profile_init)
