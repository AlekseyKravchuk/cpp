# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.28)
project(client_server_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Используем библиотеку Boost::program_options для парсинга аргументов командной строки

# Если в системе несколько версий Boost
#set(BOOST_ROOT "/path/to/specific/boost")
# ИЛИ
#set(BOOST_INCLUDE_DIR "/path/to/specific/boost/include")
#set(BOOST_LIBRARY_DIR "/path/to/specific/boost/lib")

# ============ Параметризуем версию Boost ============
set(BOOST_VERSION "1.75.0" CACHE STRING "Minimum Boost version")

# ================= Подключаем Boost =================
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS program_options)

# TODO: подключить библиотеки libnl и libnl-genl

if (Boost_FOUND)
    message(STATUS "Found Boost version: ${Boost_VERSION}")
    message(STATUS "Include directories: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# Для того, чтобы заголовки Boost были доступны глобально для всех целей
include_directories(${Boost_INCLUDE_DIRS})

# Добавляем общую библиотеку shared
add_library(shared STATIC
        shared/wrappers.cpp
        shared/wrappers.h
        shared/utilities.cpp
        shared/utilities.h)

# Указываем, где искать заголовочные файлы для shared
target_include_directories(shared PUBLIC ${CMAKE_SOURCE_DIR}/shared)

# Подключаем подкаталоги клиента и сервера
add_subdirectory(client)
add_subdirectory(server)

