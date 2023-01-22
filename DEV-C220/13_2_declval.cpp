#include <type_traits>  // std::is_same_v
#include <utility>      // std::declval
#include <vector>

// #include "my_print.h"

// DECLVAL - шаблон функции std::declval(), которая преобразует любой тип T к "rvalue-reference" (T&&),
// что позволяет использовать методы класса в "decltype" без использования конструкторов

// поскольку в структуре "А" программистом не реализовано никаких конструкторов,
// то default-конструктор будет сгенерирован компилятором
struct A {
    int fA() const {
        return 1;
    }
};

// в структуре "B" нет default-конструктора
// и компилятор НЕ сгенерирует его, поскольку программистом реализован конструктор копирования
struct B {
    // конструктор копирования
    B(const B&) {}

    int fB() const {
        return 2;
    }
};

int main() {
    decltype(A().fA()) nA = 1;
    static_assert(std::is_same_v<decltype(nA), int>);

    // decltype(B().fB()) nB = 2;
    // static_assert(std::is_same_v<decltype(nB), int>);

    // выкрутились через использование "std::declval"
    decltype(std::declval<B>().fB()) nB = 2;
    static_assert(std::is_same_v<decltype(nB), int>);

    // ещё можно так выкрутиться
    // "ноль" приводим к указателю на "B" (B*) и посредством этого указателя вызываем нужный нам public-метод
    // decltype(reinterpret_cast<B*>(0)->fB()) nB = 2;
    // static_assert(std::is_same_v<decltype(nB), int>);

    return 0;
}
