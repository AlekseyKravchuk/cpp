#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    int i = 2'000'000'000;
    // unsigned i = 2'000'000'000;
    std::cout << "i = "s << i << std::endl;

    // переполнение знакового целого, 4'000'000'000 не помещается в "int"
    // int имеет размер 4 байта: -2^31 до (2^31 - 1)
    i *= 2;
    std::cout << "i = "s << i << std::endl;

    return 0;
}
