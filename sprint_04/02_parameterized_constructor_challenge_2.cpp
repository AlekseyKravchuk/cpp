#include <cstdlib>  // for std::abs
#include <iostream>
#include <numeric>  // for std::gcd
#include <string>
#include <vector>

using namespace std;

class Rational {
   public:
    // Явно сообщаем компилятору, что в этом классе нужно создать конструктор по умолчанию
    Rational() = default;

    // Параметризованный конструктор
    // Если в классе объявлен параметризованный конструктор, конструктор по умолчанию для этого класса сгенерирован не будет.
    // Наличие конструктора с параметрами как бы сообщает компилятору: "Этот класс требует особой инициализации".
    // Если конструктор по умолчанию всё-таки нужен, вы можете написать его сами либо попросить компилятор.
    // Для этого вместо тела конструктора укажите = default и задайте полям значения по умолчанию.
    Rational(int numerator, int denominator) {
        int GCD = gcd(numerator, denominator);
        if (GCD) {
            _numerator = abs(numerator / GCD);
            _denominator = (denominator / GCD);
        } else {
            Rational();
        }
    }

    Rational(int numerator) {
        _numerator = numerator;
        _denominator = 1;
    }

    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

   private:
    // Задаем значения по умолчанию для числителя и знаменателя
    int _numerator = 0;
    int _denominator = 1;
};

Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();
    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{numerator, denominator};
}

// ===================================== Tests =====================================
#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define RUN_TEST(func)                  \
    {                                   \
        RunTestImpl(func);              \
        cerr << #func << " OK" << endl; \
    }

template <typename FuntionType>
void RunTestImpl(FuntionType func) {
    func();
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str,
                     const string& file, const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

void AssertImpl(bool expr, const string& exprAsStr,
                const string& fileName, const string& functionName,
                const int lineNumber, const string& hint) {
    if (!expr) {
        cout << fileName << "("s << lineNumber << "): "s
             << functionName << ": "s
             << "ASSERT("s << exprAsStr << ") failed."s;

        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

void TestConvertFractionToIrreducibleForm() {
    Rational two_sixths(2, 6);  // дробь 2/6
    ASSERT(two_sixths.Numerator() == 1);
    ASSERT(two_sixths.Denominator() == 3);

    Rational minus_two_sixths(-2, 6);  // дробь -2/6
    ASSERT(two_sixths.Numerator() == 1);
    ASSERT(two_sixths.Denominator() == 3);

    Rational zero;  // дробь 0/1 = 0
    ASSERT(zero.Numerator() == 0);
    ASSERT(zero.Denominator() == 1);

    Rational three_twelfths(3, 12);  // дробь 3/12
    Rational res1 = Add(three_twelfths, minus_two_sixths);  // 3/12 + (-2/6)
    ASSERT(res1.Numerator() == 7);
    ASSERT(res1.Denominator() == 12);
}

// Функция TestFractions является точкой входа для запуска тестов
void TestFractions() {
    RUN_TEST(TestConvertFractionToIrreducibleForm);
}
// ===================================== End of Tests =====================================

void main_part_1() {
    Rational zero;                   // Дробь 0/1 = 0
    const Rational seven(7);         // Дробь 7/1 = 7
    const Rational one_third(1, 3);  // Дробь 1/3
    vector<Rational> numbers;
    numbers.push_back(Rational{7, 8});

    // Следующие 2 строки эквивалентны - добавляют в numbers дробь 3/1
    numbers.push_back(Rational{3});
    numbers.push_back(3);
    Rational sum = Add(Rational{1, 6}, one_third);

    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator() << endl;
}

int main() {
    TestFractions();
}



