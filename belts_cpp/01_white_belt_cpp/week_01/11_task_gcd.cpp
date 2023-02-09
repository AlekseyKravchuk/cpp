#include <algorithm>  // std::swap
#include <iostream>
#include <string>
#include <utility>

using namespace std::literals;

/*
В stdin даны два натуральных числа. Выведите в stdout их наибольший общий делитель.
 */

int gcd(int a, int b) {
    if (a < b) {
        std::swap(a, b);
    }

    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }

    return a;
}

int main() {
    int a{}, b{};
    std::cin >> a >> b;
    std::cout << gcd(a, b) << std::endl;
    // std::cout << "gcd("s << a << ", "s << b << ") = "s << gcd(a, b) << std::endl;

    return 0;
}
