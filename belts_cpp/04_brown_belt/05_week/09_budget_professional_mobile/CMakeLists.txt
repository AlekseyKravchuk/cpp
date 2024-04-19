cmake_minimum_required(VERSION 3.28)
project(09_budget_professional_mobile)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0")

set(CMAKE_BUILD_TYPE Debug)


set(SOURCES
        budget_professional_mobile.cpp)

set(HEADERS
        test_runner.h
        profile.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

