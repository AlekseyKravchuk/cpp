#include <cassert>
#include <cmath> // std::abs, std::pow
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

// с целочисленными значениями всё плохо, т.к. MAX value for uint64_t = 18446744073709551615,
// очень быстро случается buf oveflow
long double fast_pow(long double x, int n) {
    bool is_negative = n < 0;
    int64_t power = static_cast<int64_t>(n);

    if (is_negative) {
        power = -power;
    }

    long double res = 1.0;
    long double mult = x;

    while (power) {  // пока не убили все биты в двоичном представлении числа "power"
        if (power & 0x1) {
            res *= mult;
        }

        mult *= mult;
        power /= 2;
    }

    return res;
}

// TODO: implement stress test
// Реализовать сравнение двух переменных типа "double" с помощью "double reasonable_error = 1e-6;"
// уже реализовано в ./sprint_05/20_queries_queue.cpp:                      

void StressTest() {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 rng(rd()); // Standard Mersenne-Twister engine (mersenne_twister_engine) seeded with rd()
    int min_power{0}, max_power{30};
    uint32_t min_base{0}, max_base{15};
    std::uniform_int_distribution<int> uni_pow(min_power, max_power); // Guaranteed unbiased
    std::uniform_int_distribution<int> uni_base(min_base, max_base);

    const size_t NUM_ITERATIONS = 200;
    for (size_t i = 0; i < NUM_ITERATIONS; ++i) {
        uint64_t x = uni_base(rng);
        uint64_t n = uni_pow(rng);

        uint64_t res = fast_pow(x, n);
        uint64_t expected = static_cast<uint64_t>(std::pow(static_cast<double>(x), static_cast<double>(n)));

        if (res != expected) {
            cout << "fast_pow(" << x << ", " << n << ") = " << res << " != "
                 << "std::pow(" << x << ", " << n << ") = " << expected << endl;
            break;
        }
    }
}

int main() {
    // StressTest();

    // uint64_t x = 5;
    // uint64_t n = 19;
    // // 19073486328125
    // uint64_t res = fast_pow(x, n);
    // cout << x << "^" << n << " = " << res << endl;

    // cout << "MAX value for unsigned: " << numeric_limits<unsigned>::max() << endl;
    // cout << "MIN value for unsigned: " << numeric_limits<unsigned>::min() << endl;

    // cout << "============================" << endl;

    // cout << "MAX value for uint: " << numeric_limits<uint>::max() << endl;
    // cout << "MIN value for uint: " << numeric_limits<uint>::min() << endl;

    // cout << "============================" << endl;

    // cout << "MAX value for long: " << numeric_limits<long>::max() << endl;
    // cout << "MIN value for long: " << numeric_limits<long>::min() << endl;

    cout << "MAX value for uint64_t: " << numeric_limits<uint64_t>::max() << endl;
    cout << "MIN value for uint64_t: " << numeric_limits<uint64_t>::min() << endl;
    cout << "============================" << endl;

    cout << "MAX value for long: " << numeric_limits<long>::max() << endl;
    cout << "MIN value for long: " << numeric_limits<long>::min() << endl;
    cout << "============================" << endl;

    cout << "MAX value for int64_t: " << numeric_limits<int64_t>::max() << endl;
    cout << "MIN value for int64_t: " << numeric_limits<int64_t>::min() << endl;
    cout << "============================" << endl;

    cout << "MAX value for double: " << std::fixed << std::setprecision(2) << numeric_limits<double>::max() << endl;
    cout << "MIN value for double: " << std::fixed << std::setprecision(2) << numeric_limits<double>::min() << endl;
    cout << "============================" << endl;

    cout << "MAX value for long double: " << std::fixed << std::setprecision(2) << numeric_limits<long double>::max() << endl;
    cout << "MIN value for long double: " << std::fixed << std::setprecision(2) << numeric_limits<long double>::min() << endl;
    cout << "============================" << endl;

    return 0;
}
