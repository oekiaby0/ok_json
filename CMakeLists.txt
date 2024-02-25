cmake_minimum_required(VERSION 3.21)
project(ok_json LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(
        ok_json main.cpp ok_json.cpp ok_json.hpp lexer.cpp lexer.hpp
        main.cpp)
target_compile_features(ok_json PRIVATE cxx_std_20)

install(TARGETS ok_json)