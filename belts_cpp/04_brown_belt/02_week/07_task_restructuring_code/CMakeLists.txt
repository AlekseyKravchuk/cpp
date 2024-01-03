cmake_minimum_required(VERSION 3.24)
project(13_task_restructuring_code)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(13_task_restructuring_code
        main.cpp
        profile.h
        stats_aggregator.cpp
        stats_aggregator.h
        stats_aggregator_test.cpp
        test_runner.h)
