cmake_minimum_required(VERSION 3.28)
project(05_transport_guide_part_E_TEMPORAL_V2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0")
set(CMAKE_BUILD_TYPE Debug)

include_directories(.)
#add_subdirectory(00_inputs_and_outputs)

set(SOURCE_DIR src)

set(SOURCES
        svg.cpp
        json.cpp
        domain.cpp
        geo.cpp
        json_reader.cpp
        map_renderer.cpp
        request_handler.cpp
        transport_catalogue.cpp
        json_builder.cpp
        transport_router.cpp
        )

set(HEADERS
        svg.h
        json.h
        graph.h
        ranges.h
        router.h
        canvas.h
        transport_router.h
        json_builder.h
        transport_catalogue.h
        request_handler.h
        map_renderer.h
        json_reader.h
        geo.h
        domain.h
        )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


