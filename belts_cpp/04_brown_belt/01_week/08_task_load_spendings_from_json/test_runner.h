#pragma once

#include "profile.h"

#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// ================================ Testing Framework ================================
// ===================================================================================
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
// ===================================================================================

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<FirstType, SecondType>& p) {
    os << "{" << p.first << " : " << p.second << "}";

    return os;
}
// ===================================================================================

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "[" << Join(v, ", ") << "]";
}
// ===================================================================================

template <class T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& lst) {
    return os << "[" << Join(lst, ", ") << "]";
}
// ===================================================================================

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{" << Join(s, ", ") << "}";
}
// ===================================================================================

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    os << "{" << Join(m, ", ") << "}";

    return os;
}
// ===================================================================================

template <typename T, typename U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
    if (!(t == u)) {
        std::ostringstream oss;
        oss << "Assertion failed: " << t << " != " << u
            << " hint: " << hint;
        throw std::runtime_error(oss.str());
    }
}
// ===================================================================================

// для упрощения проверки булевских значений
inline void Assert(bool bool_value, const std::string& hint) {
    AssertEqual(bool_value, true, hint);
}
// ===================================================================================

class TestRunner {
  public:
    // Шаблон "RunTest" позволяет указывать блок "try-catch" только в одном месте
    // и передавать в него произвольные функции для тестирования
    template <class TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) {
        {
            if (_times_runtest_called == 0) {
                std::cerr << "======================== Beginning of TESTS ========================" << std::endl;
            } else {
                std::cerr << std::endl;
            }

            LOG_DURATION(test_name);

            try {
                func();
                ++_times_runtest_called; // учитываем очередной запуск функции-теста

                std::cerr << _times_runtest_called << ") "
                          << std::setfill('-') << std::setw(30) << std::left
                          << test_name + ' ' << " OK" << std::endl;
            } catch (std::exception& e) {
                ++_fail_count;           // увеличиваем счетчик упавших тестов
                ++_times_runtest_called; // учитываем упавший тест в общем количестве запущенных тестов
                std::cerr << test_name + ':' << " fail: " << e.what() << std::endl;
            } catch (...) {
                ++_fail_count;            // увеличиваем счетчик упавших тестов
                ++_times_runtest_called; // учитываем упавший тест в общем количестве запущенных тестов
                std::cerr << "Unknown exception caught" << std::endl;
            }
        }
    }

    // В деструкторе оцениваем количество упавших тестов и общее количество запусков тестов
    ~TestRunner() {
        if (_fail_count > 0) {
            std::cerr << "============ ATTENTION! " << _fail_count << " test(s) failed. Terminate. ============" << std::endl;
            exit(1);
        } else {
            std::cerr << "================= ALL " << _times_runtest_called << " TEST(S) SUCCESSFULLY PASSED =================" << std::endl;
        }
    }

  private:
    size_t _fail_count = 0;           // счетчик числа упавших тестов
    size_t _times_runtest_called = 0; // счетчик количества запусков метода "RunTest"
};

#ifndef UNIQ_ID

#define GENERATE(x) a_local_uniq_id__##x
#define PROXY(x) GENERATE(x)
#define UNIQ_ID PROXY(__LINE__)

#endif

#define ASSERT_EQUAL(x, y)                       \
    {                                            \
        std::ostringstream UNIQ_ID;              \
        UNIQ_ID << #x << " != " << #y << ", ";   \
        UNIQ_ID << __FILE__ << ": " << __LINE__; \
        AssertEqual(x, y, UNIQ_ID.str());        \
    }

#define ASSERT(x)                                \
    {                                            \
        std::ostringstream UNIQ_ID;              \
        UNIQ_ID << #x << " is false, ";          \
        UNIQ_ID << __FILE__ << ": " << __LINE__; \
        AssertEqual(x, true, UNIQ_ID.str());     \
    }

// tr - object of class TestRunner
// # - change name of variable into string
#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)
