cmake_minimum_required(VERSION 3.28)
project(client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -O0"
)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        simple_client.cpp
)

#set(HEADERS
#        some_header.h
#)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})

# Ссылка на библиотеку shared
target_link_libraries(client PRIVATE shared)
