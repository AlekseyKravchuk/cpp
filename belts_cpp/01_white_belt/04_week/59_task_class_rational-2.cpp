#include <iostream>
#include <numeric>  // std::gcd
#include <string>

/*
Задание по программированию «Класс Rational»
Часть 2

Реализуйте для класса Rational операторы ==, + и - так, чтобы операции с дробями можно было записывать естественным образом.
Например, следующий код должен быть валидным:

const Rational r = Rational(1, 2) + Rational(1, 3) - Rational(1, 4);
if (r == Rational(7, 12)) {
  cout << "equal";
}

Гарантируется, что во всех тестах, на которых будет тестироваться ваша реализация,
числители и знаменатели ВСЕХ дробей (как исходных, так и получившихся в результате соответствующих арифметических операций)
будут укладываться в диапазон типа int.
*/

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

int main() {
    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            std::cout << "4/6 != 2/3" << std::endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            std::cout << "2/3 + 4/3 != 2" << std::endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            std::cout << "5/7 - 2/9 != 31/63" << std::endl;
            return 3;
        }
    }

    std::cout << "OK" << std::endl;

    return 0;
}