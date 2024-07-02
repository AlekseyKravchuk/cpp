cmake_minimum_required(VERSION 3.28)
project(01_going_out_of_vector_bounds)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -O0 \
                                        -D_GLIBCXX_DEBUG \
                                        -D_GLIBCXX_DEBUG_PEDANTIC")

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        01_going_out_of_vector_bounds.cpp )

#set(HEADERS         )
#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES})
