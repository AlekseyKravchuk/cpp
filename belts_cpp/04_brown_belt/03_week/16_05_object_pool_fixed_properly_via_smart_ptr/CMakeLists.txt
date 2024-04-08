cmake_minimum_required(VERSION 3.28.3)
project(16_05_object_pool_fixed_properly_via_smart_ptr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O0")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        object_pool.cpp)

set(HEADERS
        profile.h
        test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

