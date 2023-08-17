#include <iostream>
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
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

    // В деструкторе оцениваем количество упавших тестов
    ~TestRunner() {
        if (_fail_count) {
            std::cerr << "============ ATTENTION! Some TESTS failed. Exiting! ============"s << std::endl;
            std::exit(1);
        } else {
            std::cerr << "================= ALL TESTS SUCCESSFULLY PASSED ================="s << std::endl;
        }
    }

   private:
    // счетчик числа упавших тестов
    int _fail_count = 0;

    // счетчик количества запусков метода "RunTest"
    inline static int _times_runtest_called = 0;
};

bool operator==(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() == rhs.Numerator()) &&
           (lhs.Denominator() == rhs.Denominator());
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    return {
        lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()};
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    return {
        lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()};
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    return {lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator()};
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    if (!rhs.Denominator()) {
        throw std::domain_error("Division by zero");
    }

    return lhs * Rational(rhs.Denominator(), rhs.Numerator());
}

std::istream& operator>>(std::istream& is, Rational& r) {
    int n{}, d{};
    char delim{};

    if (is >> n >> delim >> d && delim == '/') {
        r = Rational(n, d);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    os << r.Numerator() << '/' << r.Denominator();

    return os;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() * rhs.Denominator()) < (rhs.Numerator() * lhs.Denominator());
}

void TestConstructor() {
    {
        Rational r_empty;
        AssertEqual(r_empty, Rational{0, 1});
    }
}

void TestParamConstructor_RationalReduction() {
    {
        Rational r(4, 6);
        AssertEqual(r, Rational{2, 3});
    }

    {
        Rational r(-6, 8);
        AssertEqual(r, Rational{-3, 4});
    }

    {
        Rational r(6, -8);
        AssertEqual(r, Rational{-3, 4});
    }

    {
        Rational r(-6, -8);
        AssertEqual(r, Rational{3, 4});
    }
}

void TestNegativeRational() {
    {
        Rational r(6, -8);
        AssertEqual(r, Rational{-3, 4});
    }

    {
        Rational r(6, -7);
        AssertEqual(r, Rational{-6, 7});
    }

    {
        Rational r(-6, -7);
        AssertEqual(r, Rational{6, 7});
    }
}

void TestNegativeRational_If_Both_Negative() {
    {
        Rational r(-6, -8);
        AssertEqual(r, Rational{3, 4});
    }
}

void TestWhenDenominatorIsZero() {
    {
        Rational r(0, -8);
        AssertEqual(r, Rational{0, 1});
    }
}

void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(TestConstructor, "TestConstructor"s);
    tr.RunTest(TestParamConstructor_RationalReduction, "TestParamConstructor_RationalReduction"s);
    tr.RunTest(TestNegativeRational, "TestNegativeRational"s);
    tr.RunTest(TestNegativeRational_If_Both_Negative, "TestNegativeRational_If_Both_Negative"s);
    tr.RunTest(TestWhenDenominatorIsZero, "TestWhenDenominatorIsZero"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll

int main() {
    TestAll();

    return 0;
}
