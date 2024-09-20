cmake_minimum_required(VERSION 3.28)
project(client_server_sber)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Подключаем подкаталоги (subdirectories)
add_subdirectory(client)
add_subdirectory(server)

