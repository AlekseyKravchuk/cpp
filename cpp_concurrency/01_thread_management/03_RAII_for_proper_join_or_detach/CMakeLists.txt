cmake_minimum_required(VERSION 3.28)
project(03_RAII_for_proper_join_or_detach)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0 -pthread")

set(CMAKE_BUILD_TYPE Debug)


set(SOURCES
        RAII_for_proper_join_or_detach.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

