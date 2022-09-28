// ПРИМЕР.
// Вызов параметризованного конструктора при композиции

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
            _denominator = abs(denominator / GCD);
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

struct RationalPoint {
    Rational x;
    Rational y;
};

Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();
    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{numerator, denominator};
}

Rational AddRationals(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    // Компилятор знает, что функция возвращает Rational, и неявно
    // вызывает соответствующий конструктор
    return {numerator, denominator};
    // Эта запись в данном контексте аналогична:
    // return Rational{numerator, denominator};
}

int main() {
    // Компилятор знает, что функция AddRationals принимает аргументы типа Rational
    // и конструирует дроби 1/6 и 1/3
    Rational sum = AddRationals({1, 6}, {1, 3});

    // Допустимые способы объявления переменной типа Rational
    const Rational x1{7, 8};
    const Rational x2{4, 12};
    const Rational y1(3, 4);

    // Инициализируем поля структуры при помощи имеющихся переменных
    RationalPoint p1 = {x1, y1};

    // Инициализируем поля структуры явным образом
    RationalPoint p0 = {
        {2, 3},
        {5, 6},
    };

    // Совмещаем разные способы инициализации полей структуры
    const RationalPoint p2{x2, {7, 8}};
}
