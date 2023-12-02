#include <iostream>
#include <numeric>  // std::gcd
#include <stdexcept>  // std::domain_error
#include <string>

/*
Задание по программированию «Класс Rational»
Часть 3

Аналогично предыдущей части, реализуйте операторы * и /. Например, следующий код должен быть валидным:

const Rational r = Rational(1, 2) * Rational(1, 3) / Rational(1, 4);
if (r == Rational(2, 3)) {
  cout << "equal";
}

Как и в предыдущем случае, гарантируется, что во всех тестах, на которых будет тестироваться ваша реализация,
числители и знаменатели ВСЕХ дробей (как исходных, так и получившихся в результате соответствующих арифметических операций)
будут укладываться в диапазон типа int.

Кроме того, гарантируется, что ни в одном из тестов, на которых будет тестироваться ваша реализация, не будет выполняться деление на ноль.
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

Rational operator*(const Rational& lhs, const Rational& rhs) {
    return {lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator()};
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    if (!rhs.Denominator()) {
        throw std::domain_error("Division by zero");
    }

    return lhs * Rational(rhs.Denominator(), rhs.Numerator());
}

int main() {
    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            std::cout << "2/3 * 4/3 != 8/9" << std::endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            std::cout << "5/4 / 15/8 != 2/3" << std::endl;
            return 2;
        }
    }

    std::cout << "OK" << std::endl;
    return 0;
}