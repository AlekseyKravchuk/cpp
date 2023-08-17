#include <iostream>
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <typename First, typename Second>
ostream& operator<<(ostream& os, const pair<First, Second>& p) {
    os << "{" << p.first << ", " << p.second << "}"s;
    return os;
}

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

class Rational {
   public:
    Rational() = default;

    Rational(int new_numerator, int new_denominator) {
        int gcd = std::gcd(new_numerator, new_denominator);
        _numerator = new_numerator / gcd;
        _denominator = new_denominator / gcd;

        if (_denominator < 0) {
            _numerator = -_numerator;
            _denominator = -_denominator;
        }
    }

    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

   private:
    int _numerator{0};
    int _denominator{1};
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

void AssertRational(const Rational& submitted, const pair<int, int>& correct, const string& message) {
    AssertEqual(pair<int, int>(submitted.Numerator(), submitted.Denominator()), correct, message);
}

const auto p = [](int numerator, int denominator) {
    return pair<int, int>(numerator, denominator);
};

void TestPredefinedFromStepik() {
    AssertRational(Rational(1, 1), p(1, 1), "Canonical form of 1/1 is 1/1");
    AssertRational(Rational(3, 5), p(3, 5), "Canonical form of 3/5 is 3/5");
    AssertRational(Rational(2147483647, 2147483647), p(1, 1), "Canonical form of 2147483647/2147483647 is 1/1");
    AssertRational(Rational(-5, 4), p(-5, 4), "Canonical form of -5/4 is -5/4");
    AssertRational(Rational(5, -4), p(-5, 4), "Canonical form of 5/-4 is -5/4");
    AssertRational(Rational(-6, -2), p(3, 1), "Canonical form of -6/-2 is 3/1");
    AssertRational(Rational(21, 56), p(3, 8), "Canonical form of 21/56 is 3/8");
    AssertRational(Rational(0, 100), p(0, 1), "Canonical form of 0/100 is 0/1");
    AssertRational(Rational(), p(0, 1), "Canonical form of default constructed is 0/1");
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
    tr.RunTest(TestPredefinedFromStepik, "TestPredefinedFromStepik");
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll

int main() {
    TestAll();

    return 0;
}
