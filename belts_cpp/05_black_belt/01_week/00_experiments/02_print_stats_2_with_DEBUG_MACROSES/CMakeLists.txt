cmake_minimum_required(VERSION 3.28)
project(02_print_stats_2_with_DEBUG_MACROSES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -pthread -O0 \
                                        -D_GLIBCXX_DEBUG \
                                        -D_GLIBCXX_DEBUG_PEDANTIC")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        print_stats_2.cpp)

 set(HEADERS
         test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
