#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    int x = 2'000'000'000;
    int y = 1'000'000'000;
    double avg = x / 2 + y / 2;
    // double avg = (x + y) / 2;  // ERROR: переполнение int'а, поскольку промежуточные вычисления не помещаются в тип "int"
    std::cout << std::fixed << std::setprecision(2) << avg << std::endl;
    // std::cout << std::numeric_limits<int>::max() << std::endl;
    return 0;
}
