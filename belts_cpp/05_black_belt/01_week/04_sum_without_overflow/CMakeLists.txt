cmake_minimum_required(VERSION 3.28)
project(04_sum_without_overflow)

set(CMAKE_CXX_STANDARD 17)
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

#set(HEADERS
        #)

set(SOURCES
        04_sum_without_overflow.cpp
        )

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})


