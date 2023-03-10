#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    int x = -2'000'000'000;
    std::cout << "sizeof(int) = " << sizeof(int) << " bytes" << std::endl;
    std::cout << "min value of int = "s << std::numeric_limits<int>::min() << "; "s
              << "max value of int = "s << std::numeric_limits<int>::max() << std::endl;

    std::cout << std::endl << "sizeof(size_t) = " << sizeof(size_t) << " bytes" << std::endl;
    std::cout << "min value of size_t = "s << std::numeric_limits<size_t>::min() << "; "s
              << "max value of size_t = "s << std::numeric_limits<size_t>::max() << std::endl;

    // std::cout << "sizeof(int32_t) = " << sizeof(int32_t) << " bytes" << std::endl;
    // std::cout << "min value of int32_t = "s << std::numeric_limits<int32_t>::min() << "; "s
    //           << "max value of int32_t = "s << std::numeric_limits<int32_t>::max() << std::endl;

    // std::cout << std::endl
    //           << "sizeof(uint32_t) = " << sizeof(uint32_t) << " bytes" << std::endl;
    // std::cout << "min value of uint32_t = "s << std::numeric_limits<uint32_t>::min() << "; "s
    //           << "max value of uint32_t = "s << std::numeric_limits<uint32_t>::max() << std::endl;

    unsigned int y = 5;

    // ERROR: x / y = 458993459
    // Пояснение: тип "int" занимает 4 байта, а тип "size_t" занимает 8 байт =>
    //            перед выполнением операции деления компилятор
    //            приведет меньший тип (знаковый "int", 4 байта) к бОльшему типу (беззнаковому "size_t", 8 байт),
    //            а "x" содержит отрицательное значение => в результате преобразования получим МУСОР
    std::cout << "x / y = "s << x / y << std::endl;

    return 0;
}
