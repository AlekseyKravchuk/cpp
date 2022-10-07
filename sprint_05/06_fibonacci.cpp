/*
Напишите рекурсивную функцию Fibonacci, которая принимает порядковый номер числа Фибоначчи и возвращает значение этого числа.
*/

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

uint64_t Fibonacci(int n) {
    if (n < 0) {
        throw invalid_argument("Argument passed to function is negative.");
    }

    if (n < 2) {
        return n;
    }

    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

int main() {
    int n;
    cout << "Type in number to calculate factorial: ";
    cin >> n;

    cout << Fibonacci(n) << endl;

    return 0;
}
