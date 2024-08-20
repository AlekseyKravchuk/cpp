cmake_minimum_required(VERSION 3.28)
project(03_client_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Подключаем подкаталоги (subdirectories)
add_subdirectory(03_client)
add_subdirectory(03_server)

