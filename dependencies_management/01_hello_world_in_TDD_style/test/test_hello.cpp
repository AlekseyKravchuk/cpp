#include "hello.h"
#include <gtest/gtest.h>

// Тест на функцию hello()
// HelloTest   — имя группы тестов (логическое объединение похожих тестов).
// OutputCheck — имя конкретного теста внутри группы.
TEST(HelloTest, OutputCheck) {  // макрос GoogleTest, который создаёт тест "OutputCheck" внутри группы тестов "HelloTest"
    std::stringstream ss;
    hello(ss);
    EXPECT_EQ(ss.str(), "Hello, world!");
}

// Точка входа для GoogleTest
int main(int argc, char **argv) {
    // Инициализирует GoogleTest (разбирает аргументы командной строки, настраивает систему тестирования)
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();  // Запускает все зарегистрированные тесты, выводит результаты в консоль.
}
