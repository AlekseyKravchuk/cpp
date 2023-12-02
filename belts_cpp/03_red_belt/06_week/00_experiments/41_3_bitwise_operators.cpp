#include <bitset>
#include <iomanip>
#include <iostream>

int main() {
    int PORTD = 0b00000001;
    // int PORTD = 0b11111111;
    int AND_mask = 1 << 0;
    int inversed_AND_mask = ~(1 << 0);

    std::cout << std::setw(25) << std::left << "PORTD = " << std::bitset<8>(PORTD) << std::endl;
    std::cout << std::setw(25) << std::left << "AND_mask = " << std::bitset<8>(AND_mask) << std::endl;
    std::cout << std::setw(25) << std::left << "inversed_AND_mask = " << std::bitset<8>(inversed_AND_mask) << std::endl;

    int PORTD_result = PORTD & inversed_AND_mask;
    std::cout << std::setw(25) << std::left
              << "PORTD_result = " << std::bitset<8>(PORTD_result) << std::endl;

    return 0;
}
