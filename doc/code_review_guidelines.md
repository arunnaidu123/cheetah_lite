\anchor code-review-guide
# Code Review #

The review process is an analysis of the code to help improve code quality.
A good review will not only catch the usual slips/problems of style and bugs, but
should also consider the overall health of the project and how any new code fits in.
Always be on the lookout for opportunities to refactor and reuse as well as
highlighting any flaws in our assumptions in the whole architecture.

Consider the following points when code reviewing wherever possible:

## High level overview
- Verify that the feature branch is synced with the latest version of dev
- Check the files that have been changed make sense? Are files present that shouldn't be there (e.g. build products, dot files from some toos etc)?
  Note this guide assumes you have an up to date local copy of the dev branch chacked out as "dev". If you prefer you can specify "origin/dev" for the branch name)
```
    git diff dev --name-only
 ```
- Have any thirdparty dependencies been changed? They must reflect the repositories they are drawn from and cannot be changed arbirarily.
  Each dependecy will have its own rules. See the thridparty/README for more details.
- scan through the logs - are the commit messages clear?
```
    git log

- check for whitespace errors

    git diff dev --check
```
- scan through the actual changes
```
    git diff dev .
```
## Compilation
- does it compile in your environment?
- does it compile in debug and release modes?
- does it compile on the reference environment (check skabuildmaster)?
- where the new code requires an optional external dependency, does it compile when this is turned off (i.e. with cmake -DENABLE\_<dep>=false)?
- ensure there are no new warning messages in the compilation - all warning should be treated as errors without an explicit exception


## Unit Tests
- Is there a unit test for each class and the test compiles?
- If there is no code to test is there adequate documentation to explain in the unitTest header file
- Do the tests conform to the style guidelines?
- What is the coverage?
- Have all the major use cases and corner cases been considered?
- Is there any scope for genralising components of the test into the test\_utils library?
- Could the tests use existing components inside the test\_utils library?
- Do the unit tests all pass (in debug and release modes)?
- Could the code being tested be simplified (design/interfaces etc) to improve testing?
- Does valgrind report any errors running the unit tests (i.e verify there are no memory leaks)?

## Looking over the code
- does the code conform to our style guidelines
  - adequate documentaion
  - indentation
  - poor or dangerous constructs
  - all redundant includes removed?
  - includes correctly placed in header or cpp file?
  - is the header file readable, in our standard format and are there adequate comments in the code
  - variable, method and class names conform

- look out for potential problems
  - is the design modular and scalable? Does it make sense to refactor this?
  - can you see any obvious bugs? Why was this not caught by the unit test?

- Packaging
  - is this code in the right place? Should it be broken out in to its own module or merged with a different one?
  - check for circular dependencies between subpackages

## Documentation
Where new documentation has been added to public headers:
  - ensure you can build the documentation (make doc)
  - ensure no new warnings from doxygen are introduced
  - examine the generated html output to ensure it is rendered correctly and fits into the overall doc structure
