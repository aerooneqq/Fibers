cmake_minimum_required(VERSION 3.3)


set(CMAKE_CXX_STANDARD 17)

set_property(SOURCE switch.S APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
set(core_SRCS main.cpp switch.S Task.cpp Task.h ThreadPool.cpp ThreadPool.h ExecutionContext.cpp ExecutionContext.h util.h util.cpp TaskController.cpp TaskController.h typedefs.h)

add_library(fibers ${core_SRCS})
add_executable(main main.cpp)
target_link_libraries(main fibers)