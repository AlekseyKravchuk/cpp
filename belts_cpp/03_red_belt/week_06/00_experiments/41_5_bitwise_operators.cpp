#include <bitset>
#include <iomanip>
#include <iostream>

int main() {
    int PINB = 0b0000000; // обе кнопки ОТПУЩЕНЫ

    std::cout << std::setw(15) << std::left << "PORTB = " << std::bitset<8>(PINB) << std::endl;
    std::cout << std::setw(15) << std::left << "RES = " << std::bitset<8>(PINB & ((1 << 2) | (1 << 0))) << std::endl;

    std::cout << "==============================================================" << std::endl;

    PINB = 0b0000100; // нажата только кнопка №1

    std::cout << std::setw(15) << std::left << "PINB = " << std::bitset<8>(PINB) << std::endl;
    std::cout << std::setw(15) << std::left << "RES = " << std::bitset<8>(PINB & ((1 << 2) | (1 << 0))) << std::endl;

    std::cout << "==============================================================" << std::endl;

    PINB = 0b0000001; // нажата только кнопка №2

    std::cout << std::setw(15) << std::left << "PINB = " << std::bitset<8>(PINB) << std::endl;
    std::cout << std::setw(15) << std::left << "RES = " << std::bitset<8>(PINB & ((1 << 2) | (1 << 0))) << std::endl;

    std::cout << "==============================================================" << std::endl;

    return 0;
}
