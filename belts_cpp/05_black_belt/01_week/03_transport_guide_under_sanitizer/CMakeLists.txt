cmake_minimum_required(VERSION 3.28)
project(03_transport_guide_under_sanitizer)

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
        descriptions.h
        graph.h
        json.h
        requests.h
        router.h
        sphere.h
        transport_catalog.h
        transport_router.h
        utils.h
        )

set(SOURCES
        descriptions.cpp
        json.cpp
        requests.cpp
        sphere.cpp
        transport_catalog.cpp
        transport_router.cpp
        utils.cpp
        main.cpp
        )
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


