cmake_minimum_required(VERSION 3.28)
project(udp_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
#                                        -Wextra \
#                                        -Wpedantic \
#                                        -Wunused-variable \
#                                        -Wconversion \
#                                        -g \
#                                        -fno-omit-frame-pointer \
#                                        -O0")

# https://clang.llvm.org/docs/DiagnosticsReference.html
add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

# Turn warnings into errors
add_compile_options(-Werror -Wno-language-extension-token)

add_compile_options(-Wno-error=unused-command-line-argument)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        udp_server.cpp)

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
target_link_libraries(
        udp_server
        PRIVATE shared
        ${Boost_LIBRARIES})

