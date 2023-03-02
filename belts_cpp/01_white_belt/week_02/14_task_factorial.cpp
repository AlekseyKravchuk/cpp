#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Напишите функцию, которая:
    - называется Factorial;
    - возвращает int;
    - принимает int и возвращает факториал своего аргумента. Гарантируется, что аргумент функции по модулю не превышает 10.
    - Для отрицательных аргументов функция должна возвращать 1.

 */

int Factorial(int n) {
    int result{1};

    for(int i = 2; i <= n; ++i) {
        result *= i;
    }

    return result;
}

int main() {
    int n{};
    std::cin >> n;
    std::cout << Factorial(n);

    return 0;
}
