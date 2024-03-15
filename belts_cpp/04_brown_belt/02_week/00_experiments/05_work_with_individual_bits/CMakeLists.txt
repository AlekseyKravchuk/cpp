cmake_minimum_required(VERSION 3.24)
project(work_with_individual_bits)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        work_with_individual_bits.cpp)

#set(HEADERS
#        foo.h)


add_executable(${PROJECT_NAME} ${SOURCES})
