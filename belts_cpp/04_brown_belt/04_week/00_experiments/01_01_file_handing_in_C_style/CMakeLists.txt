cmake_minimum_required(VERSION 3.28)
project(01_01_file_handing_in_C_style)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        main.cpp
        )

#set(HEADERS
        #profile.h
        #test_runner.h)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})
