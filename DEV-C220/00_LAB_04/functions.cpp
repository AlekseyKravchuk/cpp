#include "functions.h"

void operator""_b(const char* str) {
    std::cout << Bin2Dec(std::stoi(str)) << std::endl;
}

std::string operator""_toBinStr(unsigned long long n) {
    return DecimalToBinary(n);
}

std::string DecimalToBinary(int n) {
    std::string str{};

    while (n != 0) {
        str += std::to_string(n % 2);
        n /= 2;
    }
    std::reverse(str.begin(), str.end());

    return str;
}