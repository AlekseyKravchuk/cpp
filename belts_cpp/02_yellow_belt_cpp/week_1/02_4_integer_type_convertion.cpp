#include <cstdint>  // для использования типов с гарантированным фиксированным размером: int8_t, uint8_t, ... int64_t, uint64_t
#include <iomanip>
#include <iostream>
#include <limits>  // для того, чтобы узнавать минимальное и максимальное значения, которое может хранить тип
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals;

int main() {

    // std::cout << std::boolalpha <<  (-1 < 1u) << std::endl;
    std::cout << std::boolalpha <<  (-1 < 1) << std::endl;

    return 0;
}
