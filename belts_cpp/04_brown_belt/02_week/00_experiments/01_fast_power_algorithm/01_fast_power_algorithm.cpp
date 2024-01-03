#include <cassert>
#include <cmath>  // std::abs
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

// returns a^n
double fast_pow(double a, int n) {
    bool is_negative = n < 0;
    uint power{0};

    if (is_negative) {
        power = static_cast<uint>(-n);
    } else {
        power = n;
    }

    double res = 1.0;   // res  = a^0
    double_t mult = a;  // mult = a^1

    while (power) {
        // проверяем, оканчивается ли число "n" в своем бинарном разложении на 1
        if (power % 2) {  // эквивалентно if(n & 0x1)
            // включаем соответствующий множитель в результирующее произведение
            res *= mult;
        }

        mult *= mult;  // вычисляем a^2, a^3, ... a^k, где "k" - количество разрядов числа "n" в его двоичном представлении
        power /= 2;    // отбрасываем последний разряд
    }

    return is_negative
               ? 1.0 / res
               : res;
}

uint64_t fast_pow(uint64_t a, uint32_t n) {
    uint64_t res = 1;   // res  = a^0
    uint64_t mult = a;  // mult = a^1

    while (n) {
        // проверяем, оканчивается ли число "n" в своем бинарном разложении на 1
        if (n % 2) {  // эквивалентно if(n & 0x1)
            // включаем соответствующий множитель в результирующее произведение
            res *= mult;
        }

        mult *= mult;  // вычисляем a^2, a^3, ... a^k, где "k" - количество разрядов числа "n" в его двоичном представлении
        n /= 2;        // отбрасываем последний разряд
    }

    return res;
}

int main() {
    // double a = 5;
    // int n = 19;

    // double a = 2.0;
    // int n = -2;

    // double a = 1.0;
    // int n = -2147483648;
    // int n = -2;

    // double res = fast_pow(a, n);
    // assert(res == 19073486328125);
    // assert(res == 0.25);
    // assert(res == 1.0);

    // cout << "5^19 = " << std::fixed << std::setprecision(2) << res << endl;
    // cout << "1^-2147483648 = " << std::fixed << std::setprecision(2) << res << endl;

    uint64_t a = 5;
    uint32_t n = 19;
    uint64_t res = fast_pow(a, n);
    cout << "2.0^-2 = " << std::fixed << std::setprecision(2) << res << endl;

    cout << "MAX value for int: " << numeric_limits<int>::max() << endl;
    cout << "MIN value for int: " << numeric_limits<int>::min() << endl;

    cout << "============================" << endl;

    cout << "MAX value for uint: " << numeric_limits<uint>::max() << endl;
    cout << "MIN value for uint: " << numeric_limits<uint>::min() << endl;

    cout << "============================" << endl;

    cout << "MAX value for long: " << numeric_limits<long>::max() << endl;
    cout << "MIN value for long: " << numeric_limits<long>::min() << endl;

    return 0;
}
