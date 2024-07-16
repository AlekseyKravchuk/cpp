cmake_minimum_required(VERSION 3.28)
project(07_transport_guide_part_G)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0")
set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
        router.h
        graph.h
        transport_router.h
        json.h
        test_runner.h
        profile.h
        svg.h
        utils.h
        descriptions.h
        requests.h
        transport_catalog.h
        tests.h
        sphere.h
        map_renderer.h
        )

set(SOURCES
        main.cpp
        requests.cpp
        transport_catalog.cpp
        sphere.cpp
        descriptions.cpp
        map_renderer.cpp
        svg.cpp
        json.cpp
        tests.cpp
        transport_router.cpp
        utils.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


