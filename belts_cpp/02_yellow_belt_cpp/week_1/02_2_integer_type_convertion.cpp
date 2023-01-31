#include <cstdint>  // для использования типов с гарантированным фиксированным размером: int8_t, uint8_t, ... int64_t, uint64_t
#include <iomanip>
#include <iostream>
#include <limits>   // для того, чтобы узнавать минимальное и максимальное значения, которое может хранить тип
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    int x = 2'000'000'000;
    int y = 1'000'000'000;

    // переполнение типа, поскольку промежуточные значения тоже должны помещаться в тип
    std::cout << "x + y = "s << x + y << std::endl;

    // std::cout << "x + y = "s << x/2 + y/2 << std::endl;

    return 0;
}
