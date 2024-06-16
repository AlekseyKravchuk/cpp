cmake_minimum_required(VERSION 3.28)
project(04_transport_guide_part_D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -O0")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        transport_guide.cpp
        main.cpp
        parse.cpp
        json.cpp
        tests.cpp)

 set(HEADERS
         transport_guide.h
         parse.h
         tests.h
         json.h
         test_runner.h
         profile.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
