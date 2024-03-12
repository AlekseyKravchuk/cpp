cmake_minimum_required(VERSION 3.28.3)
project(15_task_polynomial_Yandex_version)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        polynomial.cpp polynomial.cpp polynomial.cpp)

set(HEADERS
        profile.h
        test_runner.h polynomial.cpp polynomial.cpp polynomial.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} polynomial.cpp polynomial.cpp)

