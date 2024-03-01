cmake_minimum_required(VERSION 3.22.1)

# Create a project named Tutorial. Макрос "project" установит значение для переменной ${PROJECT_NAME},
# которая будет использоваться далее как ${PROJECT_NAME}
project("std_function")

# Устанавливаем версию стандарта
set(CMAKE_CXX_STANDART 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors -O3")

# Собрать в один список все файлы по регулярному выражению
file(GLOB SOURCES_LIST *.cpp)

# Можно исходники также указать следующим образом, но тогда их нужно будет перечислять вручную:
# set (SOURCES_LIST std_function.cpp)

# Add an executable called "std_function" (${PROJECT_NAME}) to the project
add_executable (${PROJECT_NAME} ${SOURCES_LIST})



