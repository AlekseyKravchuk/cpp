cmake_minimum_required(VERSION 3.22.1)
project(12_03_Synchronized_from_scratch)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors -O0")
set(CMAKE_BUILD_TYPE DEBUG)

set(SOURCES
	12_03_Synchronized_from_scratch.cpp)

set(HEADERS
	profile.h
	test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

