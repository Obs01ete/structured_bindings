cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(structured_bindings)

add_executable(structured-bindings structured_bindings.cpp)

SET(COMPILE_FLAGS "-std=c++17")
add_definitions(${COMPILE_FLAGS})
