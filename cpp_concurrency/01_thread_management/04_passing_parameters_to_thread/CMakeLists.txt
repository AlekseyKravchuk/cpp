cmake_minimum_required(VERSION 3.28)
project(04_passing_parameters_to_thread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0 -pthread")

set(CMAKE_BUILD_TYPE Debug)


set(SOURCES
        passing_parameters_to_thread.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

