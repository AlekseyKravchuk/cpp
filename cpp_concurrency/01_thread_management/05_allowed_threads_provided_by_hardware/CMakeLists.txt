cmake_minimum_required(VERSION 3.28)
project(05_allowed_threads_provided_by_hardware)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0 -pthread")

set(CMAKE_BUILD_TYPE Debug)


set(SOURCES
        hardware_concurrency.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

