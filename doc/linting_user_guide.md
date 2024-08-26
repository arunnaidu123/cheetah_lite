\anchor linting-user-guide
# Linting User Guide

This page describes the tools and processes used for linting in the Cheetah code base.

## 1. Local Linting for Developers

This section describes the options available to developers to lint their code.

The tool used for linting is Uncrustify.

Currently there are two ways developers can choose to perform linting. The following CMake targets are available:

-# 'lint' - runs Uncrustify with configuration options set to overwrite the files with linted versions of the files.
-# 'lint_report' - runs Uncrustify with configurations options set to output linted copies of the files for developers to inspect before manually overwriting the changes to the original files.

Linting should be performed on all code before it is checked in. Developers are therefore expected to run 'lint' or 'lint_report' before checking in code. Both of these targets have been configured to use 'git ls-files' to lint all changed .cpp or .h files.

### 1.1 Lint

Below are the steps to perform to lint code where the origin files are overwritten.

-# run the following command:

        make lint

The original files are linted. A list of these files can be found in:

    <CMAKE_BINARY_DIR>/tools/uncrustify/FilesToLint.txt

### 1.2 Lint Report

Below are the steps to perform to lint code where copies of the files are produced for inspection.

-# run the following command:

        make lint_report

A copy of each of the linted files will be replaces in the following folder:

    <CMAKE_BINARY_DIR>/tools/uncrustify/output

A list of the linted files can be found in:

    <CMAKE_BINARY_DIR>/tools/uncrustify/FilesToLint.txt


## 2. Linting as part of CI/CD

TODO