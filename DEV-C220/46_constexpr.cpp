#include <chrono>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>

using namespace std::literals::chrono_literals;

int main() {
    int x = 2;

    const int i = x;  // значение "i" вычисляется в runtime
    std::cout << i << std::endl;

    // constexpr int j = x;  // ERROR: the value of ‘x’ is not usable in a constant expression
    // std::cout << j << std::endl;

    constexpr int k = 15;  // OK!
    std::cout << k << std::endl;

    return 0;
}