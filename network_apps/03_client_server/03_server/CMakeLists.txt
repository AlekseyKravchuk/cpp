cmake_minimum_required(VERSION 3.28)
project(03_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO:
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.74.0 COMPONENTS *boost libraries here*)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -O0")
set(CMAKE_BUILD_TYPE Debug)

#get_filename_component(PARENT_DIR ../ ABSOLUTE)
#include_directories(${PARENT_DIR})

set(SOURCES
        03_server.cpp
        )

#set(HEADERS
#        )

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})
