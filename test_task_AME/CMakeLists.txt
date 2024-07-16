cmake_minimum_required(VERSION 3.28)
project(test_task_AME)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -O0")
set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
        sorting_algorithms.h
        paginator.h
        test_runner.h
        profile.h
        utilities.h
        tests.h
        )

set(SOURCES
        main.cpp
        utilities.cpp
        tests.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


