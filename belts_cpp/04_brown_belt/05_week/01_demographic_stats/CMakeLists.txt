cmake_minimum_required(VERSION 3.28)
project(01_demographic_stats)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

add_definitions(-DMY_DEBUG_CONFIGURATION)

set(SOURCES
        demographic_stats.cpp)

# set(HEADERS
#         some_header.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

