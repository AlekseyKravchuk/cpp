#include <iostream>

using namespace std::literals;

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

int main() {
    // Задание 1. Реализуйте вычисление факториала с помощью constexpr-функции.
    // Подсказки/напоминания:
    //		- constexpr – функция должна состоять из единственной инструкции return <выражение>; (пока!)
    //		- но это выражение может включать вызов другой constexpr – функции, в частности рекурсивный вызов
    //		- если параметр рекурсивной constexpr- функции - это константа, компилятор вычислит результат на этапе компиляции

    // Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции (если в качествепараметра используется константа, известная компилятору на этапе компиляции).
    //  Для проверки достаточно создать встроенный массив с размерностью, вычисляемой посредством constexpr-функции:

    {
        // Например:
        // int ar[factorial(3)];

        // или
        // constexpr int n = factorial(5);
        // int ar1[n];

        // попробуйте:
        // int m = 7;
        // constexpr int n1 = factorial(m);
        // int ar1[n1];

        // а так?
        // int n2 = factorial(m);
    }

    {
        int ar[factorial(3)];
        constexpr int n = factorial(5);
        std::cout << "factorial(5) = "s << n << std::endl;
        // int ar1[n];

        const int m = 7;  // OK
        // int m = 7;     // ERROR
        constexpr int n1 = factorial(m);
        int ar1[n1];

        int n2 = factorial(m);
    }
    return 0;
}
