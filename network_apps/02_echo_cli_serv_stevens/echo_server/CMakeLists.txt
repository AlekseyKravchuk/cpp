cmake_minimum_required(VERSION 3.28)
project(echo_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -g \
                                        -O0")

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        server.cpp
        wrap_sock.cpp
        error.cpp
        wrap_unix.cpp
        str_echo.cpp
        write_n_bytes.cpp)

set(HEADERS
        unp.h
        config.h )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})