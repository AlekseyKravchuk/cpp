# Подключаем GoogleTest
find_package(GTest REQUIRED)

# Создаем исполнимый файл для тестов
#add_executable(test_hello ${CMAKE_SOURCE_DIR}/test/test_hello.cpp)
add_executable(test_hello test_hello.cpp)

# Линкуем с hello.cpp (просто добавляем исходники)
target_sources(test_hello PRIVATE ${CMAKE_SOURCE_DIR}/src/hello.cpp)

# Указываем директорию с заголовочными файлами
target_include_directories(test_hello PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Линкуем с GoogleTest
target_link_libraries(test_hello PRIVATE GTest::GTest GTest::Main)

# Добавляем тест
include(GoogleTest)
gtest_discover_tests(test_hello)