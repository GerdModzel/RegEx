add_executable(RegExCmd main.cpp)
target_compile_features(RegExCmd PRIVATE cxx_std_20)

target_include_directories(libRegEx PUBLIC ../inc)

target_link_libraries(RegExCmd PRIVATE libRegEx)

