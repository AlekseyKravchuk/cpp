#include <cstdint>  // для использования типов с гарантированным фиксированным размером: int8_t, uint8_t, ... int64_t, uint64_t
#include <iomanip>
#include <iostream>
#include <limits>   // для того, чтобы узнавать минимальное и максимальное значения, которое может хранить тип
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    auto var = std::numeric_limits<int>::max();
    std::cout << "var = "s << var << std::endl;

    // переполнение типа
    std::cout << "var + 1 = "s << var + 1 << std::endl;

    return 0;
}
