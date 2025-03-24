cmake_minimum_required(VERSION 3.28)

project(hello_with_tdd)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++-20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wthread-safety \
                                        -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -pthread \
                                        -g \
                                        -fsanitize=thread \
                                        -O3")

# Добавляем подкаталоги
add_subdirectory(src)
add_subdirectory(test)




