cmake_minimum_required(VERSION 3.28)
project(02_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -O0")
set(CMAKE_BUILD_TYPE Debug)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})

set(SOURCES
        server.cpp
        ../wrappers.cpp)

set(HEADERS
        ../wrappers.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
