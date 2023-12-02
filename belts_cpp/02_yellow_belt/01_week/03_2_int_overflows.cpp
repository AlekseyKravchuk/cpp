#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    int x = -2'000'000'000;
    unsigned int y = x;

    std::cout << x << " " << y << std::endl;

    return 0;
}
