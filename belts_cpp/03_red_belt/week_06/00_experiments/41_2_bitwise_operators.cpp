#include <bitset>
#include <iostream>

int main() {
    int PORTD = 0b00000001;
    // int PORTD = 0b11111111;

    for (size_t i = 0; i < 8u; ++i) {
        std::cout << "PORTD << " << i << " = "
                  << "0b" << std::bitset<8>(PORTD << i) << std::endl;
        std::cout << "PORTD << " << i << " = " << (PORTD << i) << std::endl
                  << std::endl;
    }

    return 0;
}
