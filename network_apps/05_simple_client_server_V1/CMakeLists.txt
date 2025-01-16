cmake_minimum_required(VERSION 3.28)
project(client_server_simple)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем общую библиотеку shared
add_library(shared STATIC
        shared/wrappers.cpp
        shared/wrappers.h)

# Указываем, где искать заголовочные файлы для shared
target_include_directories(shared PUBLIC ${CMAKE_SOURCE_DIR}/shared)

# Подключаем подкаталоги клиента и сервера
add_subdirectory(simple_client)
add_subdirectory(simple_server)

