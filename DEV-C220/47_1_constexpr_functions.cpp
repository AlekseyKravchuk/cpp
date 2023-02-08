#include <chrono>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>

using namespace std::literals::chrono_literals;

constexpr int sum(int a, int b) {
    return a + b;
}

int main() {
    constexpr int n = sum(55, 20);

    std::cout << n << std::endl;
    std::cout << sum(5, 15) << std::endl;

    return 0;
}