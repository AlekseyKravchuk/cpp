#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <numeric>  // std::accumulate

using namespace std::literals;

double CumulativeAverage(std::vector<int> v) {
    double average = 0.0;

    for (size_t n = 0; n < v.size(); ++n) {
        average += (v[n] - average) / (n + 1);
    }

    return average;
}

int main() {
    std::vector<int> v{15, 3};

    // int avg = CumulativeAverage(v);
    // std::cout << "average = "s << avg << std::endl;

    // второй вариант вычисления
    std::cout << std::accumulate(v.begin(), v.end(), 0.0,
                                 [n = 0](auto cma, auto i) mutable {
                                     return cma + (i - cma) / ++n;
                                 }) << std::endl;

    return 0;
}
