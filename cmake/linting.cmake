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

# ---------------------------------------------
# Linting
# -------
#
# Linting introduces two new targets for to perform an in place lint and
#         one to generate a report on linting errors found.
#         A supported linting tool must be enabled or the targets generated
#         will produce a default warning message about linting being deactivated.
#
# VARIABLES AFFECTING BEHAVIOUR
# -----------------------------
# LINT_COMPARISON_BRANCH : by default the files to be linted will be those that
#                          have changed relative to the beanch specified by this var
#                          By default this will be set to "origin/dev".
#                          Note that this depends on the git command being available
#                          and that the project is managed by git
#
# Adding a new linting tool
# -------------------------
# Any new tool should add custom targets defined by the variables:
# - LINTING_TARGET
# = LINTING_REPORT_TARGET
#
# The files that should be linted are those generated using the FILES_TO_LINT_COMMAND
# Each tool should have its own .cmake file in the cmake directory
# to search for its executable and setup the required targets.
# If the tool is successfully found and the targets are succesfully configured
# then the LINTING_TOOL_FOUND should be set (in PARENT_SCOPE)
# ---------------------------------------------

# -- target names for linting tools ------------
# produce a report on linting errors found without altering the original files
set(LINTING_REPORT_TARGET "lint_report")

# automatically update files to conform to linting rules
set(LINTING_TARGET "lint")

# ----------------------------------------------
if(NOT LINT_COMPARISON_BRANCH)
    set(LINT_COMPARISON_BRANCH origin/dev)
endif()
set(FILES_TO_LINT_COMMAND  git diff-index --name-only --ignore-submodules --relative --diff-filter=d ${LINT_COMPARISON_BRANCH} *.cpp *.h :!thirdparty)

# -- search for a supported linting tool
include(uncrustify)

# -- if the linting tool is not found create some dummy targets
if(NOT LINTING_TOOL_FOUND)
    set(linting_error_msg
        "No linting targets configured. Rebuild with 'cmake -DENABLE_UNCRUSTIFY=TRUE.'"
    )
    add_custom_target(${LINTING_REPORT_TARGET}
                      COMMAND ${CMAKE_COMMAND} -E echo
                      COMMENT ${linting_error_msg} VERBATIM
    )
    add_custom_target(${LINTING_TARGET}
                      COMMAND ${CMAKE_COMMAND} -E echo
                      COMMENT ${linting_error_msg} VERBATIM
    )
endif()
