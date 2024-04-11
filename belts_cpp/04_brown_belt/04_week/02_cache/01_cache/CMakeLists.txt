cmake_minimum_required(VERSION 3.28)
project(01_cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        main.cpp
        Solution.cpp)

set(HEADERS
        profile.h
        test_runner.h
        Common.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
