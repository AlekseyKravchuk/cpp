cmake_minimum_required(VERSION 3.28)
project(03_client_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(03_client_server
               03_client/03_client.cpp
               03_server/03_server.cpp
        )
