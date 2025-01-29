cmake_minimum_required(VERSION 3.28)
project(10_proxy_object)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -pthread -O0 \
                                        -g \
                                        -O0")

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        main.cpp
        option.cpp)

 set(HEADERS
         option.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

