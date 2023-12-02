#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Написать программу вычисления стоимости покупки товара с учётом скидки.

Если исходная стоимость товара больше A рублей, то на неё устанавливается скидка в X процентов.
Если исходная стоимость товара больше B рублей, то на неё устанавливается скидка в Y процентов.

На вход программе даны пять вещественных чисел: N, A, B, X, Y (A < B) - где N - исходная стоимость товара.
Выведите стоимость покупки товара с учётом скидки.
 */

int main() {
    double N, A, B, X, Y, discountedCost;
    std::cin >> N >> A >> B >> X >> Y;

    if (N <= A) {
        discountedCost = N;
    }

    if (N > A) {
        discountedCost = N - (N * X / 100.0);
    }

    if (N > B) {
        discountedCost = (N - N * Y / 100.0);
    }

    std::cout << discountedCost << std::endl;

    return 0;
}
