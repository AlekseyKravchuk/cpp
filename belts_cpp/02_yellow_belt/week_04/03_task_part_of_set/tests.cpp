#include "tests.h"

#include "find_greatest.h"

void MainTest() {
    {
        std::vector<int> v_desired = {};
        auto v = FindGreaterElements(std::set<int>{}, 5);
        AssertEqual(v_desired, v, "empty vector"s);
    }

    {
        std::vector<int> v_desired = {7};
        auto v = FindGreaterElements(std::set<int>{7}, 5);
        AssertEqual(v_desired, v, "vector containing 1 element above border"s);
    }

    {
        std::vector<int> v_desired = {};
        auto v = FindGreaterElements(std::set<int>{3}, 5);
        AssertEqual(v_desired, v, "vector containing 1 element below border"s);
    }
    {
        std::vector<int> v_desired = {5};
        auto v = FindGreaterElements(std::set<int>{1, 2, 3, 4, 5}, 4);
        AssertEqual(v_desired, v, "vector = [5]"s);
    }

    {
        std::vector<int> v_desired = {5, 7, 8};
        auto v = FindGreaterElements(std::set<int>{1, 5, 7, 8}, 3);
        AssertEqual(v_desired, v, "vector = [5, 7, 8]"s);
    }

    {
        std::vector<int> v_desired = {5, 6};
        auto v = FindGreaterElements(std::set<int>{3, 5, 6}, 4);
        AssertEqual(v_desired, v, "vector = [5, 6]"s);
    }
}

// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(MainTest, "MainTest"s);

}  // По этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner",
   // в котором произойдет обработка количества упавших тестов.
   // Для своевременного вызова деструктора "TestRunner::~TestRunner" мы и используем функцию TestAll
