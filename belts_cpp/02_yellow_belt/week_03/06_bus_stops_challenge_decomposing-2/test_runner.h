#pragma once

#include <iostream>
#include <string>

using namespace std::literals;

// Класс "TestRunner" управляет запуском unit-тестов. Создаем объект класса "TestRunner".
// Вызываем посредством этого объекта метод "RunTest" по числу имеющихся тестов.
// После того, как все тесты будут выполнены через "RunTest", нужно будет выполнить некоторые дейстия, а именно:
// посчитать количество упавших тестов и если оно больше нуля, то завершить работу программы.
// Указанный функционал реализован в деструкторе "~TestRunner".
class TestRunner {
   public:
    template <typename FuncType>
    void RunTest(FuncType test_as_func, std::string test_name) {
        try {
            if (!_times_runtest_called) {
                std::cerr << "====================== Beginning of TESTS ======================"s << std::endl;
            }
            test_as_func();
            ++_times_runtest_called;
            std::cerr << test_name << " PASSED"s << std::endl;
        } catch (const std::exception& e) {
            std::cerr << test_name << " fail: "s << e.what() << std::endl;
            ++_times_runtest_called;  // также нужно учитывать вызовы "RunTest", завершившихся падением
            ++_fail_count;
        }
    }

    ~TestRunner();

   private:
    // счетчик числа упавших тестов
    int _fail_count = 0;

    // счетчик количества запусков метода "RunTest"
    inline static int _times_runtest_called = 0;
};