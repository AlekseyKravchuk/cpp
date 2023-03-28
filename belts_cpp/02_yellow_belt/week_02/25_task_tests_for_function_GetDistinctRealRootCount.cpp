#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

template <class T>
ostream& operator<<(ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator<<(ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template <class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
   public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

   private:
    int fail_count = 0;
};

// The type-safe C++ version:
// возможный возвращаемые значения: -1, 0, +1
template <typename T>
inline int sgn(T val) {
    return ((T{} < val) - (val < T{}));
}

double GetDiscriminant(double a, double b, double c) {
    return b * b - 4 * a * c;
}

int GetDistinctRealRootCount_LinearEquation(double b, double c) {
    return (b != 0) ? 1 : 0;
}

int GetDistinctRealRootCount_QuadraticEquation(double a, double b, double c) {
    switch (sgn(GetDiscriminant(a, b, c))) {
        case -1:
            return 0;
        case 0:
            return 1;
        case 1:
            return 2;
    }
    return std::numeric_limits<unsigned int>::max();
}

int GetDistinctRealRootCount(double a, double b, double c) {
    // Вы можете вставлять сюда различные реализации функции,
    // чтобы проверить, что ваши тесты пропускают корректный код
    // и ловят некорректный
    return (a == 0) ? GetDistinctRealRootCount_LinearEquation(b, c) : GetDistinctRealRootCount_QuadraticEquation(a, b, c);
}

void TestGetDistinctRealRootCount() {
    {
        auto [a, b, c] = tuple{0, 2, 4};
        AssertEqual(1, GetDistinctRealRootCount(a, b, c), "[a, b, c] = tuple{0, 2, 4}"s);
    }

    {
        auto [a, b, c] = tuple{0, 0, 4};
        AssertEqual(0, GetDistinctRealRootCount(a, b, c), "[a, b, c] = tuple{0, 0, 4}"s);
    }

    {
        auto [a, b, c] = tuple{3, 5, 4};
        AssertEqual(0, GetDistinctRealRootCount(a, b, c), "[a, b, c] = tuple{3, 5, 4}, D < 0"s);
    }

    {
        auto [a, b, c] = tuple{1, -8, 15};
        AssertEqual(2, GetDistinctRealRootCount(a, b, c), "[a, b, c] = tuple{1, -8, 15}, D > 0"s);
    }

    {
        auto [a, b, c] = tuple{1, 2, 1};
        AssertEqual(1, GetDistinctRealRootCount(a, b, c), "[a, b, c] = tuple{1, 2, 1}, D == 0"s);
    }
}

int main() {
    TestRunner runner;
    // добавьте сюда свои тесты
    runner.RunTest(TestGetDistinctRealRootCount, "TestGetDistinctRealRootCount"s);
    return 0;
}
