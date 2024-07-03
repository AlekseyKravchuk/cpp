cmake_minimum_required(VERSION 3.28)
project(03_transport_guide_under_sanitizer_Yandex_Solution)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -pthread -O0 \
                                        -D_GLIBCXX_DEBUG \
                                        -D_GLIBCXX_DEBUG_PEDANTIC \
                                        -g \
                                        -O0 \
                                        -fsanitize=address \
                                        -fno-omit-frame-pointer \
                                        -fno-optimize-sibling-calls \
                                        ")

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
        router.h
        dbjson.h
        dbio.h
        graph.h
        busdb.h
        geometry.h
        json.h
        )

set(SOURCES
        geometry.cpp
        busdb.cpp
        main.cpp
        json.cpp
        dbio.cpp
        dbjson.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


