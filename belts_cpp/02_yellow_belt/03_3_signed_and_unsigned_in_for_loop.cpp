#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    std::vector<int> x = {4, 5, -8, 2};

    // warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’}
    // for (int i = 0; i < x.size(); ++i) {
    //     std::cout << i << " " << x[i] << std::endl;
    // }

    /*используем size_t (беззнаковый)*/
    // for (size_t i = 0; i < x.size(); ++i) {
    //     std::cout << i << " " << x[i] << std::endl;
    // }

    // или так (кастуем к знаковому):
    for (int i = 0; i < static_cast<int>(x.size()); ++i) {
        std::cout << i << " " << x[i] << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
