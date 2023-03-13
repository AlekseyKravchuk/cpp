#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    std::vector<int> t = {-8, -7, 3};
    // std::vector<int> t = {8, 7, 3};
    int sum = 0;

    for (int x : t) {
        sum += x;
    }

    // "sum" - знаковое, "t.size()" - беззнаковое
    // приведение ЗНАКОВОГО числа к БЕЗЗНАКОВОМУ (меньший по размеру тип приводится к большему типу: int => unsigned int)
    // int avg = sum / t.size();

    int avg = sum / static_cast<int>(t.size());
    std::cout << "avg = " << avg << std::endl;

    // std::cout << (-1 < 1u) << std::endl;

    return 0;
}
