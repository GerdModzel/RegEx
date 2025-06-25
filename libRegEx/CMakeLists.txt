file(GLOB HEADER_LIST CONFIGURE_DEPENDS "../inc/*.h")

add_library(libRegEx parse.cpp ParseResult.cpp NfaState.cpp NfaBuilder.cpp Expression.cpp calculateHitMask.cpp ${HEADER_LIST})

target_include_directories(libRegEx PUBLIC ../inc)

target_compile_features(libRegEx PUBLIC cxx_std_20)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/inc"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

