enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Tests need to be added as executables first
add_executable(testLibRegEx 
               src/testFind.cpp
               src/storageUtils.cpp 
               src/testExpression.cpp 
               src/testCalculateHitMask.cpp 
               src/testOperator.cpp 
               src/testExpressionBuildFunctions.cpp 
               src/testParseResult.cpp 
               src/testNfaBuilder.cpp)

# I'm using C++17 in the test
target_compile_features(testLibRegEx PRIVATE cxx_std_20)

# Should be linked to testing library
target_link_libraries(testLibRegEx PRIVATE libRegEx GTest::gtest_main)

target_include_directories(testLibRegEx PRIVATE ../inc inc)

include(GoogleTest)
gtest_discover_tests(testLibRegEx)
