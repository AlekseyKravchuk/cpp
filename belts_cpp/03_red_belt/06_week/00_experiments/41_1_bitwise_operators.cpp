#include <bitset>
#include <iostream>

int main() {
    // int a = 0b00000001;
    // int a = 1 << 0;
    // int a = 1 << 1  | 1 << 0;
    uint32_t PORTD = 0x0;
    PORTD |= (1 << 3) | (1 << 2) | (1 << 1);  // PORTD = 00001110

    // Пусть теперь в PORTD = 0b11100000 нужно 6-ой бит сбросить в ноль
    // Это значить, что нужно применить следующую AND-маску: 0b10111111
    PORTD = 0b11100000;
    std::cout << "Before resetting bit 6 to zero, PORTD = " << std::bitset<8>(PORTD) << std::endl;

    PORTD &= ~(1 << 6);
    std::cout << "AFTER resetting bit 6 to zero,  PORTD = " << std::bitset<8>(PORTD) << std::endl;

    std::cout << "======================================================" << std::endl;

    // Пусть теперь в PORTD = 0b11111111 нужно инвертировать следующие биты: 0, 2, 5, 7
    // В XOR-маске выставляем '1' в тех позициях, которые нужно инвертировать.
    //                                                         76543210
    // Это значить, что нужно применить следующую XOR-маску: 0b10100101
    PORTD = 0b11111111;
    std::cout << "PORTD = " << std::bitset<8>(PORTD) << std::endl;
    std::cout << "Toggling bits: 0, 2, 5, 7" << std::endl;
    PORTD ^= (1 << 7) | (1 << 5) | (1 << 2) | (1 << 0);
    std::cout << "PORTD = " << std::bitset<8>(PORTD) << std::endl;

    // std::cout << std::bitset<8 * sizeof(a)>(a) << std::endl;
    // std::cout << std::bitset<8>(a) << std::endl;
    // std::cout << std::bitset<8>(PORTD) << std::endl;

    return 0;
}
