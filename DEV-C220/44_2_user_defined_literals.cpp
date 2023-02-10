#include <iostream>
#include <string>

void operator"" _printDecimal(unsigned long long n) {
    std::cout << n;
}

void operator"" _printDecimal(const char* str, size_t n) {
    std::cout << str;
}

int main() {
    0x1122eeff_printDecimal;    // вывод: 287502079
    std::cout << std::endl;
    "0x1122eeff"_printDecimal;  // вывод: 0x1122eeff

    return 0;
}
