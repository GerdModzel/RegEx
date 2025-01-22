# CMakeList.txt : CMake project for RegEx, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.24)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RegEx")

# Add source to this project's executable.
add_subdirectory(libRegEx)
add_subdirectory(RegExCmd)
add_subdirectory(test)

#target_include_directories(libRegEx PRIVATE ${CMAKE_SOURCE_DIR}/inc)
#target_include_directories(RegExCmd PRIVATE ${CMAKE_SOURCE_DIR}/inc)
#target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/inc)



