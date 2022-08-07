cmake_minimum_required(VERSION 3.23)
project(untitled CXX ASM)

set(CMAKE_CXX_STANDARD 17)

set_property(SOURCE switch.s APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
add_executable(untitled main.cpp switch.s)
