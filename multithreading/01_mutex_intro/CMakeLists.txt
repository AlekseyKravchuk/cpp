cmake_minimum_required(VERSION 3.28)
project(01_mutex_intro)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wthread-safety")

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        main.cpp)

# set(HEADERS
#         thread_annotations.h)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})

