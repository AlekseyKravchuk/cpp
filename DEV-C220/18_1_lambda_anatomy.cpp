#include <cmath>  // std::pow
#include <iostream>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "my_print.h"

using namespace std::literals;

// возможная реализация библиотечного алгоритма "std::for_each"
// в списке параметров шаблонной функции "pred" - это ОБЪЕКТ типа Predicate (функтор)
// ОБЪЕКТ типа функтор называют CLOSURE,
// а сам ФУНКТОР называют CLOSURE CLASS
template <typename It, typename Predicate>
void my_for_each(It it, It itPastTheLast, Predicate pred) {
    while (it != itPastTheLast) {
        pred(*it);  // functorObject.operator()(*it);
        ++it;
    }
}

// реализуем ФУНКТОР явно
// возводит в куб каждый переданный в CLOSURE(объект типа Functor<T>) отрицательный элемент
template <typename T>
struct Functor {
    void operator()(T& x) const {
        if (x < 0) {
            x = std::pow(x, 3);
        }
    }
};

// Рассмотрим анатомию этой странной записи
// int n = [](int x, int y){ return x + y; }(5, 4);

// Эквивалентно:
// Вручнаю объявили класс. Нам от этого класса нужен только "operator()"
// Этот "operator()" (оператор "круглые скобки") будет принимать два параметра типа "int"
// Метод класса "Anonim" объявлен константным, поскольку
// внутренний механизм лямбды генерирует "под капотом" именно КОНСТАНТНЫЙ "operator()"
class Anonim {
   public:
    int operator()(int _x, int _y) const { return _x + _y; }

    Anonim() {
        std::cout << "Default constructor of class Anonim was called." << std::endl;
    }

    Anonim(const Anonim&) {
        std::cout << "Copy constructor of class Anonim was called." << std::endl;
    }

    Anonim(Anonim&&) {
        std::cout << "Move constructor of class Anonim was called." << std::endl;
    }
};

// То, что мы сделали руками в с классом "Anonim", компилятор делает автомитически при использовании ЛЯМБД.
// То есть, компилятор в месте использования ЛЯМБДЫ генерирует безымянный класс (или структуру) и перегружает в нем "operator()"
// !!! перегружаемый "operator()" для ЛЯМБД всегда КОНСТАНТНЫЙ
// => в теле этого метода: 1) могут вызываться только КОНСТАНТНЫЕ методы; 2) нельзя модифицировать переменные класса

int main() {
    // Требуется с помощью my_for_each распечатать куб каждого отрицательного элемента
    std::vector<int> v{5, -7, 4, -3, 11, -2};
    PrintCollection(v, "Initial state of vector v: "s);

    // в списке параметров "my_for_each" выражение Functor<int>()
    // означает создать объект типа "Functor<int>" и проинициализировать его default-конструктором (конструктором по умолчанию)
    // my_for_each(v.begin(), v.end(), Functor<int>());

    // то же самое, но посредством ЛЯМБДЫ
    my_for_each(v.begin(), v.end(), [](int& val) {
        if (val < 0) {
            val = std::pow(val, 3);
        }
    });
    PrintCollection(v, "Vector v after applying predicate: "s);

    return 0;
}
