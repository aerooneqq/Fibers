cmake_minimum_required(VERSION 3.3)

project(Test)

include(CTest)
enable_testing()

set(REPO ~/ClionProjects/.repo)

set(gtest_disable_pthreads on)
include(${REPO}/DownloadProject/DownloadProject.cmake)
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             main
        UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set(test_SRCS gtest.cpp ThreadPoolTests.cpp)
add_executable(runUnitTests ${test_SRCS})
target_link_libraries(runUnitTests gtest gmock fibers)