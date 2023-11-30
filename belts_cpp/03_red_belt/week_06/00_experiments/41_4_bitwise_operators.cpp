#include <bitset>
#include <iomanip>
#include <iostream>

int main() {
    int PORTB = 0b11101101; // обе кнопки ОТПУЩЕНЫ
    int AND_mask = (1 << 2) | (1 << 0);

    std::cout << std::setw(15) << std::left << "PORTB = " << std::bitset<8>(PORTB) << std::endl;
    std::cout << std::setw(15) << std::left << "AND_mask = " << std::bitset<8>(AND_mask) << std::endl;

    int PORTB_result = PORTB & AND_mask;

    std::cout << std::setw(15) << std::left
              << "PORTB_result = " << std::bitset<8>(PORTB_result) << std::endl;

    std::cout << "==============================================================" << std::endl;

    PORTB = 0b01010100; // нажата только кнопка №1

    std::cout << std::setw(15) << std::left << "PORTB = " << std::bitset<8>(PORTB) << std::endl;
    std::cout << std::setw(15) << std::left << "AND_mask = " << std::bitset<8>(AND_mask) << std::endl;

    PORTB_result = PORTB & AND_mask;

    std::cout << std::setw(15) << std::left
              << "PORTB_result = " << std::bitset<8>(PORTB_result) << std::endl;

    std::cout << "==============================================================" << std::endl;

    PORTB = 0b01010001; // нажата только кнопка №2

    std::cout << std::setw(15) << std::left << "PORTB = " << std::bitset<8>(PORTB) << std::endl;
    std::cout << std::setw(15) << std::left << "AND_mask = " << std::bitset<8>(AND_mask) << std::endl;

    PORTB_result = PORTB & AND_mask;

    std::cout << std::setw(15) << std::left
              << "PORTB_result = " << std::bitset<8>(PORTB_result) << std::endl;

    std::cout << "==============================================================" << std::endl;

    PORTB = 0b11110000; // нажаты ОБЕ кнопки

    std::cout << std::setw(15) << std::left << "PORTB = " << std::bitset<8>(PORTB) << std::endl;
    std::cout << std::setw(15) << std::left << "AND_mask = " << std::bitset<8>(AND_mask) << std::endl;

    PORTB_result = PORTB & AND_mask;

    std::cout << std::setw(15) << std::left
              << "PORTB_result = " << std::bitset<8>(PORTB_result) << std::endl;

    return 0;
}
