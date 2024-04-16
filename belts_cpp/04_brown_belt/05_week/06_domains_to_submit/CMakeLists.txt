cmake_minimum_required(VERSION 3.28)
project(06_domains_to_submit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O0")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        main.cpp)


add_executable(${PROJECT_NAME} ${SOURCES})

