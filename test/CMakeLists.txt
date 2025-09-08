file(GLOB HEADER_LIST CONFIGURE_DEPENDS "../test/inc/*.h" "../test/inc/op/*.h" "../test/nfa/inc/*.h")

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
               src/op/testOperator.cpp 
               src/testExpressionBuildFunctions.cpp 
               src/testParseResult.cpp 
               src/nfa/MainBuilderForTest.cpp
               src/nfa/testMainBuilder.cpp
               src/nfa/testFragmentSearch.cpp
	           ${HEADER_LIST})

# I'm using C++20 in the test
target_compile_features(testLibRegEx PRIVATE cxx_std_20)

# Should be linked to testing library
target_link_libraries(testLibRegEx PRIVATE libRegEx GTest::gtest_main)

target_include_directories(testLibRegEx PRIVATE ../inc inc)

include(GoogleTest)
gtest_discover_tests(testLibRegEx)

# IDEs should put the headers in a nice place
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})



