#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>  // std::pair
#include <vector>

// ================================ Testing Framework ================================
template <typename Collection>
std::string Join(const Collection& collection, const std::string& separator = " ") {
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
    os << "{" << p.first << " : " << p.second << "}";

    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "[" << Join(v, ", ") << "]";
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    os << "{" << Join(m, ", ") << "}";

    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{" << Join(s, ", ") << "}";
}

template <typename T, typename U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
    if (!(t == u)) {
        std::ostringstream oss;
        oss << "Assertion failed: " << t << " != " << u
            << " hint: " << hint;
        throw std::runtime_error(oss.str());
    }
}

// для упрощения проверки булевских значений
inline void Assert(bool bool_value, const std::string& hint) {
    AssertEqual(bool_value, true, hint);
}

class TestRunner {
   public:
    // Шаблон "RunTest" позволяет указывать блок "try-catch" только в одном месте и передавать в него произвольные функции для тестирования
    template <typename TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) {
        try {
            if (_times_runtest_called == 0) {
                std::cerr << "======================== Beginning of TESTS ========================" << std::endl;
            }

            func();
            ++_times_runtest_called;  // учитываем очередной запуск функции-теста

            std::cerr << _times_runtest_called << ") "
                      << std::setfill('-') << std::setw(30) << std::left
                      << test_name << " OK" << std::endl;
        } catch (const std::runtime_error& e) {
            ++_fail_count;            // увеличиваем счетчик упавших тестов
            ++_times_runtest_called;  // учитываем упавший тест в общем количестве запущенных тестов
            std::cerr << test_name << " fail: " << e.what() << std::endl;
        }
    }

    // В деструкторе оцениваем количество упавших тестов и общее количество запусков тестов
    ~TestRunner() {
        if (_fail_count) {
            std::cerr << "============ ATTENTION! " << _fail_count << " test(s) failed. Terminate. ============" << std::endl;
            exit(1);
        } else {
            std::cerr << "================= ALL " << _times_runtest_called << " TEST(S) SUCCESSFULLY PASSED =================" << std::endl;
        }
    }

   private:
    size_t _fail_count = 0;            // счетчик числа упавших тестов
    size_t _times_runtest_called = 0;  // счетчик количества запусков метода "RunTest"
};

// Чтобы создавать многострочные макросы, каждую строчку макроса кроме последней, нужно завершать обратным слешем "\"
#define ASSERT_EQUAL(x, y) {                \
        std::ostringstream oss;             \
        oss << #x << " != " << #y << ", "   \
            << __FILE__ << ":" << __LINE__; \
        AssertEqual(x, y, oss.str());       \
}

#define ASSERT(x) {                         \
        std::ostringstream oss;             \
        oss << #x << " is false, "          \
            << __FILE__ << ":" << __LINE__; \
        Assert(x, oss.str());               \
}

// оператор '#' возвращает строковое представление параметра
#define RUN_TEST(tr_object, func) \
    tr_object.RunTest(func, #func)
// ================================ END of Testing Framework ================================