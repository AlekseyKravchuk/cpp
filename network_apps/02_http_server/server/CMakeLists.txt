cmake_minimum_required(VERSION 3.28)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                        -Wunused-variable \
                                        -Wconversion \
                                        -Wextra \
                                        -pedantic-errors \
                                        -g \
                                        -O0")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        server.cpp)

#set(HEADERS)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})

# Связываем целевой исполняемый файл server с библиотеками "shared" и значением переменной ${Boost_LIBRARIES}.
# 1) server — это имя целевой программы, которая создаётся с помощью команды add_executable.
#      Напр., add_executable(server server.cpp) указывает, что server компилируется из файла server.cpp.
# 2) ${Boost_LIBRARIES} — это переменная, предоставляемая CMake после вызова find_package(Boost ...).
#      Она содержит список библиотек, необходимых для использования модулей Boost, указанных в параметре find_package.
#      Напр., для Boost::program_options это может быть что-то вроде -lboost_program_options.


# обе библиотеки и shared, и Boost — будут добавлены к цели server.
target_link_libraries(server
        PRIVATE shared
        ${Boost_LIBRARIES})

