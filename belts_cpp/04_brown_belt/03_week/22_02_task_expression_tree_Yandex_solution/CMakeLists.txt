cmake_minimum_required(VERSION 3.28)
project(22_02_task_expression_tree_Yandex_solution)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        expression_tree.cpp)

set(HEADERS
        Common.h
        test_runner.h
        profile.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
