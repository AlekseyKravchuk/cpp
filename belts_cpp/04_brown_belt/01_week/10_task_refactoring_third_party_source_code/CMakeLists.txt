cmake_minimum_required(VERSION 3.24)
project(10_task_refactoring_third_party_source_code)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(10_task_refactoring_third_party_source_code
        json.cpp
        json.h
        profile.h
        refactoring.cpp
        test_runner.h
        xml.cpp
        xml.h)
