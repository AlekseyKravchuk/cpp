#include <algorithm>
#include <iostream>
#include <string>

constexpr int Bin2Dec(int n, int i = 0) {
    return n ? (n % 10) * (1 << i) + Bin2Dec(n / 10, i + 1) : 0;
}

void operator""_b(const char* str);
std::string operator""_toBinStr(unsigned long long n);

std::string DecimalToBinary(int n);

// template <typename T>
// constexpr auto Smth() {
    
// }