cmake_minimum_required(VERSION 3.28)
project(04_demographic_stats_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        demographic_stats_tests.cpp)

set(HEADERS
        test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
