# The MIT License (MIT)
#
# Copyright (c) 2022 The SKA organisation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#=======================================================================
# Find and setup the uncrustify linting tool
#
# brief: if the uncrustify executable is found, will set the the
#        linting targets to use uncrustify with the config file
#        specfied in the ${UNCRUSTIFY_CONFIG} variable.
#
# Variables
# ^^^^^^^^^
#
#     ``UNCRUSTIFY_EXE``
#       Path to the uncrustify executable.
#       Can be set on the command line or will be set if cmake can find it.
#
#     ``UNCRUSTIFY_VERSION``
#       Version of uncrustify selected.
#
#     ``UNCRUSTIFY_VERSION_MAJOR``
#       Major version of uncrustify.
#
#     ``UNCRUSTIFY_VERSION_MINOR``
#       Minor version of uncrustify.
#
#     ``UNCRUSTIFY_VERSION_PATCH``
#       Patch version of uncrustify.
#
#     ``LINTING_TOOL_FOUND``
#       Set to TRUE if a valid version of uncrustify was found.
#
#=======================================================================
option(ENABLE_UNCRUSTIFY "Enable use of the UNCRUSTIFY linting tool." OFF)

if(NOT SKA_CHEETAH_UNCRUSTIFY_GUARD_VAR)
    set(SKA_CHEETAH_UNCRUSTIFY_GUARD_VAR TRUE)
else()
    return()
endif()

if(ENABLE_UNCRUSTIFY)

    # Check user input if provided on the command line
    if(DEFINED UNCRUSTIFY_EXE)
        get_property(is_cached CACHE UNCRUSTIFY_EXE PROPERTY TYPE)
        if(NOT ${is_cached})
            if(NOT EXISTS ${UNCRUSTIFY_EXE})
                message(FATAL_ERROR "Specified UNCRUSTIFY_EXE (${UNCRUSTIFY_EXE}) does not exist")
            endif()
        endif()
    endif()

    # Find the program
    find_program(UNCRUSTIFY_EXE uncrustify
                 DOC "The uncrustify linting tool executable"
    )

    if(UNCRUSTIFY_EXE AND NOT (UNCRUSTIFY_EXE EQUAL "UNCRUSTIFY_EXE-NOTFOUND"))
        # Runs the command to get the version
        execute_process(COMMAND ${UNCRUSTIFY_EXE} --version
                        OUTPUT_VARIABLE uncrustify_version
        )
        string(REGEX REPLACE "^[a-zA-Z_-]*(..*)(..)" "\\1" UNCRUSTIFY_VERSION ${uncrustify_version})
        include(semantic_version)
        extract_semantic_version("UNCRUSTIFY")

        if(${UNCRUSTIFY_VERSION_MAJOR} EQUAL 0)
            if(${UNCRUSTIFY_VERSION_MINOR} LESS_EQUAL 75)
                if(${UNCRUSTIFY_VERSION_PATCH} LESS 1)
                    message(FATAL_ERROR "Got uncrustify version ${UNCRUSTIFY_VERSION}.  Minimum version supported = 0.75.1")
                    return()
                endif()
            endif()
        endif()

        set(uncrustify_home_dir "${PROJECT_SOURCE_DIR}/tools/uncrustify")
        set(uncrustify_build_dir "${CMAKE_BINARY_DIR}/tools/uncrustify")

        # Get the config file which contains the set of rules to apply when running uncrustify
        if(NOT UNCRUSTIFY_CONFIG)
            set(UNCRUSTIFY_CONFIG ${uncrustify_home_dir}/config/uncrustify_0_75_1.cfg)
        endif()

        if(NOT EXISTS ${UNCRUSTIFY_CONFIG})
            message(FATAL_ERROR "Error: Uncrustify config file not found at location: ${UNCRUSTIFY_CONFIG}")
        else()
            set(uncrustify_output_dir ${uncrustify_build_dir}/output)
            file(MAKE_DIRECTORY ${uncrustify_output_dir})

            set(uncrustify_files_list ${uncrustify_build_dir}/FilesToLint.txt)
            set(create_file_list_command ${FILES_TO_LINT_COMMAND} > ${uncrustify_files_list})

            add_custom_target(${LINTING_REPORT_TARGET}
                              COMMAND ${create_file_list_command}
                              COMMAND ${UNCRUSTIFY_EXE}
                                  -c ${UNCRUSTIFY_CONFIG}
                                  -F ${uncrustify_files_list}
                                  --prefix ${uncrustify_output_dir}
                              WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                              COMMENT "Uncrustify has linted files specified in ${uncrustify_files_list}
                                       and output them to ${uncrustify_output_dir}." VERBATIM
            )

            add_custom_target(${LINTING_TARGET}
                              COMMAND ${create_file_list_command}
                              COMMAND ${UNCRUSTIFY_EXE}
                                      -c ${UNCRUSTIFY_CONFIG}
                                      -F ${uncrustify_files_list}
                                      --replace
                                      --no-backup
                              WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                              COMMENT "Uncrustify has linted files specified in ${uncrustify_files_list}
                                       and overwritten the original files." VERBATIM
            )
            set(LINTING_TOOL_FOUND TRUE)
        endif()

    endif()

endif()
