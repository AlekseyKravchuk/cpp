cmake_minimum_required(VERSION 3.28)
project(01_unique_ptr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        unique_ptr.cpp)

set(HEADERS
        profile.h
        test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

