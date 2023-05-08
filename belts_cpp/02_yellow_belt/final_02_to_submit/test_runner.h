#pragma once

#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std::literals;

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<FirstType, SecondType>& p);

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s);

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m);

void Assert(bool bool_value_to_check, const std::string& hint = {});

// AssertEqual должна сравнивать аргументы любых типов
template <typename T, typename U>
void AssertEqual(T t, U u, const std::string hint = {}) {
    if (t != u) {
        std::ostringstream oss;
        oss << "Assertion failed: " << t << " != " << u
            << ", Hint: " << hint;
        throw std::runtime_error(oss.str());
    }
}

// Класс "TestRunner" управляет запуском unit-тестов. Создаем объект класса "TestRunner".
// Вызываем посредством этого объекта метод "RunTest" по числу имеющихся тестов.
// После того, как все тесты будут выполнены через "RunTest", нужно будет выполнить некоторые дейстия, а именно:
// посчитать количество упавших тестов и если оно больше нуля, то завершить работу программы.
// Указанный функционал реализован в деструкторе "~TestRunner".
class TestRunner {
   public:
    template <typename FuncType>
    void RunTest(FuncType test_func, std::string test_name);
    ~TestRunner();  // В деструкторе оцениваем количество упавших тестов

   private:
    int _fail_count = 0;                          // счетчик числа упавших тестов
    inline static int _times_runtest_called = 0;  // счетчик количества запусков метода "RunTest"
};

template <typename Collection>
std::string Join(const Collection& collection, const std::string separator = " "s) {
    std::ostringstream oss;
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << separator;
        }
        isFirst = false;
        oss << elm;
    }

    return oss.str();
}

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<FirstType, SecondType>& p) {
    return os << p.first << ": " << p.second;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "["s << Join(v, ", "s) << "]"s;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{"s << Join(s, ", "s) << "}"s;
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    return os << "{"s << Join(m, ", "s) << "}"s;
}

template <typename FuncType>
void TestRunner::RunTest(FuncType test_func, std::string test_name) {
    try {
        if (!_times_runtest_called) {
            std::cerr << "====================== Beginning of TESTS ======================"s << std::endl;
        }
        test_func();
        ++_times_runtest_called;
        std::cerr << test_name << " PASSED"s << std::endl;
    } catch (const std::exception& e) {
        std::cerr << test_name << " fail: "s << e.what() << std::endl;
        ++_times_runtest_called;  // также нужно учитывать вызовы "RunTest", завершившихся падением
        ++_fail_count;
    }
}