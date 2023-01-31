#include <cstdint>  // для использования типов с гарантированным фиксированным размером: int8_t, uint8_t, ... int64_t, uint64_t
#include <iomanip>
#include <iostream>
#include <limits>  // для того, чтобы узнавать минимальное и максимальное значения, которое может хранить тип
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals;

int main() {
    int x = 2'000'000'000;
    unsigned int y = 1'000'000'000;

    // переполнения типа не происходит, поскольку срабатывает правило:
    // если оба операнда имеют одинаковые размеры, но разную "знаковость", то результат будет БЕЗЗНАКОВЫМ
    // переменная "res" будет иметь тип "unsigned int"
    auto res = x + y;
    // static_assert(std::is_same_v<decltype(res), int>);

    std::cout << "x + y = "s << res << std::endl;

    // std::cout << "x + y = "s << x/2 + y/2 << std::endl;

    return 0;
}
