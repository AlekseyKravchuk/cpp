#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    uint64_t x = 1024;
    std::vector<uint32_t> degrees = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "x = " << x << std::endl;

    for (auto degree : degrees) {
        std::cout << x << " / 2 ^ " << degree << " = " << (x >> degree) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "x = " << x << std::endl;
    std::cout << "y = x / 2 = " << (x >> 1) << std::endl;
    std::cout << "y = x / 2 ^ 2 = " << (x >> 2) << std::endl;

    return 0;
}
