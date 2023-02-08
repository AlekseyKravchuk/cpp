#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Дано два целых числа A и B (A <= B, A >= 1, B <= 30000). Выведите через пробел все чётные числа от A до B (включительно).
Для проверки целого числа x на чётность используется операция взятия остатка от деления на 2, которая в C++ оформляется с помощью символа "%".
 */

void PrintEvenNumbersInRange(int A, int B) {
    int start_from = A;

    if (A % 2 != 0) {
        if (A + 1 <= B) {
            start_from = A + 1;
        } else {
            return;
        }
    }

    while (start_from <= B) {
        std::cout << start_from << ' ';
        start_from += 2;
    }
}

int main() {
    int A, B;
    std::cin >> A >> B;

    PrintEvenNumbersInRange(A, B);

    return 0;
}
