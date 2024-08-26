include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIBRARY_DIR})

set(POLICY_TESTS_SRC
    src/FlagPolicyTest.cpp
    src/LastUnflaggedTest.cpp
    PARENT_SCOPE
)
