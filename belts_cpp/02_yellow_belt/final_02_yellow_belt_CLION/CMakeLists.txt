cmake_minimum_required(VERSION 3.23)
project(yellow_belt_final)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(yellow_belt_final
        condition_parser.cpp
        condition_parser.h
        database.cpp
        database.h
        date.cpp
        date.h
        input_redirection.cpp
        input_redirection.h
        main.cpp
        node.cpp
        node.h
        test_runner.cpp
        test_runner.h
        token.cpp
        token.h tests.h tests.cpp)
