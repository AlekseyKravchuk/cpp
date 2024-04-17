cmake_minimum_required(VERSION 3.28)
project(domains)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O0")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        main.cpp
        domains.cpp
        tests.cpp)

set(HEADERS
        domains.h
        tests.h
        test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


