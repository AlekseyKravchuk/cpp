cmake_minimum_required(VERSION 3.24)
project(22_01_task_expression_tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        expression_tree.cpp)

set(HEADERS
        Common.h
        test_runner.h
        profile.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

