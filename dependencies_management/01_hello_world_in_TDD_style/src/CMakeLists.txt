# CMakeLists.txt для каталога src
cmake_minimum_required(VERSION 3.28)

# Добавляем все файлы из src в список исходников
set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hello.cpp)

# Создаем исполняемый файл из исходников
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем директорию include, чтобы исходники видели заголовочные файлы
# PRIVATE означает, что только этот таргет (исполнимый файл) использует заголовочные файлы
# Это помогает избежать утечек зависимостей
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)