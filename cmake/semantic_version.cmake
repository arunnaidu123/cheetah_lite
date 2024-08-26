if(NOT SKA_CHEETAH_SEMANTIC_VERSION_GUARD_VAR)
    set(SKA_CHEETAH_SEMANTIC_VERSION_GUARD_VAR TRUE)
else()
    return()
endif()

#========================================================
# extract_semantic_version
#
# brief: will split the MY_PACKAGE_VERSION variable into its
#        components (seperated by ".", "-", or "_" characters)
#
# usage: extract_semantic_version(<my_package>)
#
# detail: This function will create the following variables:
#
#             ``MY_PACKAGE_VERSION_LIST``
#               A list of the components.
#
#             ``MY_PACKAGE_VERSION_MAJOR``
#               The major version.
#
#             ``MY_PACKAGE_VERSION_MINOR``
#               The minor version.
#
#             ``MY_PACKAGE_VERSION_PATCH``
#               The patch version.
#
#========================================================
function(extract_semantic_version package_name)
    if(NOT DEFINED ${package_name}_VERSION)
        return()
    else()
        string(REGEX REPLACE "[-|.|_]" ";" version_list ${${package_name}_VERSION})
    endif()
    if(version_list)
        list(LENGTH version_list list_length)
        if(${list_length} GREATER 0)
            list(GET version_list 0 version_major)
            set(${package_name}_VERSION_MAJOR ${version_major} PARENT_SCOPE)
            if(${list_length} GREATER 1)
                list(GET version_list 1 version_minor)
                set(${package_name}_VERSION_MINOR ${version_minor} PARENT_SCOPE)
                if(${list_length} GREATER 2)
                    list(GET version_list 2 version_patch)
                    set(${package_name}_VERSION_PATCH ${version_patch} PARENT_SCOPE)
                endif()
            endif()
        endif()
        set(${package_name}_VERSION_LIST ${version_list} PARENT_SCOPE)
    endif()
endfunction(extract_semantic_version)
