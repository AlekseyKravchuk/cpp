cmake_minimum_required(VERSION 3.28)
project(02_personal_budget_under_sanitizer)

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
                                        -fno-omit-frame-pointer \
                                        -fno-optimize-sibling-calls \
                                        -fsanitize=address")

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        02_personal_budget_under_sanitizer.cpp)

# set(HEADERS
#         some_header.h)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES})

