#include <iostream>
#include <numeric>  // std::gcd
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>

/*
Задание по программированию «Класс Rational»
Часть 4

В этой части вам нужно реализовать операторы ввода и вывода для класса Rational. В результате у вас должен работать, например, такой код

Rational r;
cin >> r;
cout << r;
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

int main() {
    {
        std::ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            std::cout << "Rational(-6, 8) should be written as \"-3/4\"" << std::endl;
            return 1;
        }
    }

    {
        std::istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            std::cout << "5/7 is incorrectly read as " << r << std::endl;
            return 2;
        }
    }

    {
        std::istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            std::cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << std::endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            std::cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << std::endl;
            return 4;
        }
    }

    std::cout << "OK" << std::endl;
    return 0;
}