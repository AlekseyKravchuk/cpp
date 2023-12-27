cmake_minimum_required(VERSION 3.24)
project(01_trie_testing)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(01_trie_testing
        join_collection.h
        prefix_tree_constructing.cpp
        profile.h
        test_runner.h)
