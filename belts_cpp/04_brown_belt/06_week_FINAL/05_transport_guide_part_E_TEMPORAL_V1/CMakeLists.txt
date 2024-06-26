cmake_minimum_required(VERSION 3.28)
project(05_transport_guide_part_E_TEMPORAL_V1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0")
set(CMAKE_BUILD_TYPE Debug)


set(SOURCES
        bus.cpp
        stop_manager.cpp
        transport_manager.cpp
        json.cpp
        json_parser.cpp transport_guide.cpp)

set(HEADERS
         bus.h
         stop_manager.h
         transport_manager.h
         transport_manager_command.h
         json.h
         json_parser.h
         graph.h
         router.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


