cmake_minimum_required(VERSION 3.28.3)
project(13_task_synchronized_2_with_constness)

set(CMAKE_CXX_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")

set(CMAKE_BUILD_TYPE Debug)
# or
# set(CMAKE_BUILD_TYPE Release)


set(SOURCES
    13_task_synchronized_2_with_constness.cpp)

set(HEADERS
    profile.h
    test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
