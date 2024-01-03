cmake_minimum_required(VERSION 3.24)
project(12_from_final_yellow_belt)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(12_from_final_yellow_belt
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
        profile.h
        test_runner.h
        tests.cpp
        tests.h
        token.cpp
        token.h)
