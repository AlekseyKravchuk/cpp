#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>  // std::accumulate
#include <vector>

using namespace std::literals;

double CumulativeAverage(std::vector<int> v) {
    double average = 0.0;

    for (size_t n = 0; n < v.size(); ++n) {
        average += (v[n] - average) / static_cast<int>((n + 1));
    }

    return average;
}

int main() {
    // std::vector<int> v{5, 4, 1, -2, 7};
    std::vector<int> v{5, 4, 9};

    // int avg = CumulativeAverage(v);
    // std::cout << "average = "s << avg << std::endl;

    // второй вариант вычисления
    std::cout << std::accumulate(v.begin(), v.end(), 0.0,
                                 [n = 0](auto cma, auto x) mutable {
                                     cma = cma + (x - cma) / ++n;
                                     return cma;
                                 })
              << std::endl;

    return 0;
}
