# The MIT License (MIT)
#
# Copyright (c) 2016 The SKA organisation
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

if(NOT SKA_CHEETAH_DOXYGEN_GUARD_VAR)
    set(SKA_CHEETAH_DOXYGEN_GUARD_VAR TRUE)
else()
    return()
endif()

if(CMAKE_BUILD_TYPE MATCHES documentation)
    set(ENABLE_DOC TRUE)
    set(doc_all_target "ALL")
else()
    set(doc_all_target "")
endif()

# Search for the DoxyfileAPI.in by looking through CMAKE_MODULE_PATH
# This will pull it, in order of preference, from either the local
# cmake modules, the installed panda, or from the panda source
find_file(doxyfile_in "DoxyfileAPI.in" ${CMAKE_MODULE_PATH})

message("Doxyfile set to " ${doxyfile_in})

set(project_doc_target "doc_${PROJECT_NAME}")

if(ENABLE_DOC)

    find_package(Doxygen)

    # Add a target to generate API documentation with Doxygen
    if(DOXYGEN_FOUND)
        set(doxygen_build_dir ${CMAKE_BINARY_DIR}/doc)
        file(MAKE_DIRECTORY ${doxygen_build_dir})
        configure_file(${doxyfile_in} ${CMAKE_BINARY_DIR}/DoxyfileAPI @ONLY)
        add_custom_target(${project_doc_target}
                          ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/DoxyfileAPI
                          WORKING_DIRECTORY ${doxygen_build_dir}
                          COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
        install(DIRECTORY ${doxygen_build_dir}/
                DESTINATION ${DOC_INSTALL_DIR}
                PATTERN "${doxygen_build_dir}/*"
        )
    else()
        add_custom_target(${project_doc_target}
                          COMMAND ${CMAKE_COMMAND} -E echo
                          COMMENT "No doc target configured. Doxygen not found" VERBATIM
        )
    endif()

else()
    add_custom_target(${project_doc_target}
                      COMMAND ${CMAKE_COMMAND} -E echo
                      COMMENT "No doc target configured. Rebuild with 'cmake -DENABLE_DOC=true .'" VERBATIM
    )
endif()

list(APPEND PROJECT_DOC_TARGETS ${project_doc_target})

get_directory_property(has_parent PARENT_DIRECTORY)

if(has_parent)
    set(PROJECT_DOC_TARGETS ${PROJECT_DOC_TARGETS} PARENT_SCOPE)
else()
    set(PROJECT_DOC_TARGETS ${PROJECT_DOC_TARGETS})
endif()

# Add top-level doc target only when there can be no clashes
if("${PROJECT_NAME}" STREQUAL "${CMAKE_PROJECT_NAME}")
    add_custom_target(doc ${doc_all_target} DEPENDS ${PROJECT_DOC_TARGETS})
endif()
