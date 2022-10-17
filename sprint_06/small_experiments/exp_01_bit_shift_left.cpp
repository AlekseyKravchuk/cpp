#include <iostream>

int main() {
    int testVal = 15;

    int res1 = testVal << 1;
    std::cout << "Value \"" << testVal << "\" shifted 1 bit left is equal to: " << res1 << std::endl;

    int res2 = testVal << 2;
    std::cout << "Value \"" << testVal << "\" shifted 2 bits left is equal to: " << res2 << std::endl;

    int res3 = testVal << 3;
    std::cout << "Value \"" << testVal << "\" shifted 3 bits left is equal to: " << res3 << std::endl;

    return 0;
}
