#include <cstdint>  // для использования типов с гарантированным фиксированным размером: int8_t, uint8_t, ... int64_t, uint64_t
#include <iomanip>
#include <iostream>
#include <limits>   // для того, чтобы узнавать минимальное и максимальное значения, которое может хранить тип
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    std::cout << "sizeof(uint64_t): " << sizeof(uint64_t) << " bytes." << std::endl;
    std::cout << "min(uint64_t) = " << std::setw(10) << std::left << std::numeric_limits<uint64_t>::min() << "\n"s
              << "max(uint64_t) = " << std::setw(10) << std::numeric_limits<uint64_t>::max() << std::endl;

    std::cout << std::endl;

    std::cout << "sizeof(size_t): " << sizeof(size_t) << " bytes." << std::endl;
    std::cout << "min(size_t) = " << std::setw(10) << std::left << std::numeric_limits<size_t>::min() << "\n"s
              << "max(size_t) = " << std::setw(10) << std::numeric_limits<size_t>::max() << std::endl;

    std::cout << std::endl;

    std::cout << "sizeof(long int): " << sizeof(long int) << " bytes." << std::endl;
    std::cout << "min(long int) = " << std::setw(10) << std::left << std::numeric_limits<long int>::min() << "\n"s
              << "max(long int) = " << std::setw(10) << std::numeric_limits<long int>::max() << std::endl;

    return 0;
}
