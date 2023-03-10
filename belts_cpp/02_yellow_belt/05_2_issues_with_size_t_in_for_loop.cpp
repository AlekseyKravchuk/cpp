#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    // Пусть стоит задача вывести все элементы вектора в обратном порядке
    // std::vector<int> v = {5, 8, 11};
    std::vector<int> v;

    // ERROR:
    // выражение 'i >= 0' верно при любом 'i', поскольку 'i' - беззнаковое
    // for (size_t i = v.size() - 1; i >= 0; --i) {
    //     std::cout << "v["s << i << "]: "s << v[i] << std::endl;
    // }

    // КОСТЫЛЬ!!!
    // for (size_t k = v.size(); k > 0; --k) {
    //     size_t i = k - 1;
    //     std::cout << "v["s << i << "]: "s << v[i] << std::endl;
    // }

    // ЛУЧШЕ ТАК
    // for (int i = static_cast<int>(v.size() - 1); i >= 0; --i) {
    //     std::cout << "v["s << i << "]: "s << v[i] << std::endl;
    // }

    return 0;
}
