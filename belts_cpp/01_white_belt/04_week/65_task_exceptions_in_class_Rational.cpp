#include <iostream>
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Исключения в классе Rational»

Вспомним класс Rational, который мы реализовали в задачах, посвящённых перегрузке операторов.
Там специально говорилось, что в тестах объекты класса Rational никогда не создаются с нулевым знаменателем и никогда не выполняется деление на ноль.
Настало время избавиться от этого ограничения и научиться обрабатывать нулевой знаменатель и деление на ноль. В этой задаче вам предстоит это сделать.
Поменяйте реализацию конструктора класса Rational так, чтобы он выбрасывал исключение invalid_argument,если знаменатель равен нулю.
Кроме того, переделайте реализацию оператора деления для класса Rational так, чтобы он выбрасывал исключение domain_error, если делитель равен нулю.

Напомним, что вам надо реализовать интерфейс

class Rational {
public:
  Rational();
  Rational(int numerator, int denominator);

  int Numerator() const;
  int Denominator() const;
};

и оператор деления для класса Rational

Rational operator / (const Rational& lhs, const Rational& rhs);

Так же, как и в задачах на перегрузку операторов, вам будет предложен файл с заготовкой кода.
Функция main в этом файле содержит ряд юнит-тестов, которые позволят вам проверить свою реализацию.
При проверке вашего решения оно будет тестироваться на более полном наборе тестов,
так что успешное прохождение всех юнит-тестов не гарантирует, что ваша реализация будет зачтена.
*/

class Rational {
   public:
    Rational() = default;

    Rational(int new_numerator, int new_denominator) {
        if (new_denominator == 0) {
            throw std::invalid_argument("/* Zero denominator in constructor. */"s);
        }

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
    if (rhs.Numerator() == 0) {
        throw std::domain_error("/* Division by zero */"s);
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

int main() {
    try {
        Rational r(1, 0);
        std::cout << "Doesn't throw in case of zero denominator" << std::endl;
        return 1;
    } catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        std::cout << "Doesn't throw in case of division by zero" << std::endl;
        return 2;
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "OK" << std::endl;
    return 0;
}