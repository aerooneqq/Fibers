cmake_minimum_required(VERSION 3.23)
project(Fibers CXX ASM)

set(CMAKE_CXX_STANDARD 17)

set_property(SOURCE src/switch.asm APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
add_executable(Fibers src/main.cpp src/switch.asm src/Task.cpp src/Task.h src/ThreadPool.cpp src/ThreadPool.h src/ExecutionContext.cpp src/ExecutionContext.h src/util.h src/util.cpp)
