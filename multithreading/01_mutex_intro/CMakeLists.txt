cmake_minimum_required(VERSION 3.28)
project(01_mutex_intro)

set(CMAKE_CXX_COMPILER clang++-17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wthread-safety -DTHREAD_ANNOTATIONS")

# 1) ThreadSanitizer (TSan) используется для динамического анализа во время выполнения программы.
#    Чтобы использовать TSan, необходимо скомпилировать программу с флагами -fsanitize=thread, а затем запустить её, чтобы обнаружить гонки данных.
# 2) Clang Static Analyzer — это инструмент для статического анализа, который используется с clang-tidy или напрямую с clang++ через флаг -analyze.
#    Это может помочь выявить проблемы в коде ещё до его запуска.

## включаем динамический анализ с помощью ThreadSanitizer, который отслеживает ошибки многозадачности (например, гонки данных).
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
#
## clang-tidy с флагом -checks=-*,clang-analyzer-* позволяет включить статический анализ кода с использованием встроенных проверок Clang
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,clang-analyzer-*")  # Для использования clang-tidy и статического анализа

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        main.cpp)

# set(HEADERS
#         thread_annotations.h)

#add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})





