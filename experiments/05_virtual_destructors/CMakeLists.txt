cmake_minimum_required(VERSION 3.28)
project(05_virtual_destructors)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# "-fsanitize=undefined" option allows to run Undefined Behaviour SANitizer (UBSan).
# "-fsanitize=address" option allows to run Address Sanitizer.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -g \
                                        -O0")


add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)


set(SOURCES
        main.cpp)

# set(HEADERS
#         some_header.h)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})


