#include <bitset>
#include <cstddef>
#include <iomanip>
#include <iostream>

int main() {
    int number = -7;

    std::cout << std::left << std::setw(8) << "number" << " = " << number << std::endl;
    std::cout << std::left << std::setw(8) << "bin repr = " << std::bitset<4>(number) << std::endl;

    return 0;
}
