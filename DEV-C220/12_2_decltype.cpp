#include <iostream>
#include <string>


// начиная с С++14 trailing return type нам больше не нужен
template <typename A, typename B>
auto MySum(const A& a, const B& b) -> decltype(a + b) {
    return a + b;
}

int main() {
    // int a = 15;
    // int b = 10;

    double a = 15.2;
    double b = 10.8;

    std::cout << MySum(a, b) << std::endl;

    return 0;
}
