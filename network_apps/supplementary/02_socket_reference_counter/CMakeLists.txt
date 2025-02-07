cmake_minimum_required(VERSION 3.28)
project(04_sum_without_overflow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -g \
                                        -O0"
                                        )

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
        sock_refs_counter.h)

set(SOURCES
        sock_refs_counter.cpp
        main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
