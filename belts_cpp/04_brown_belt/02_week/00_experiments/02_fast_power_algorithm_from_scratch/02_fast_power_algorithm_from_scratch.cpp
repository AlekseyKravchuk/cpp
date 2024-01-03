#include <cassert>
#include <cmath>  // std::abs
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

uint64_t fast_pow(uint64_t x, uint64_t n) {
    if ((x < 2) || (n == 1)) {
        return x;
    }

    uint64_t res = 1;
    uint64_t mult = x;

    while (n) {
        if (n % 2) {
            res *= mult;
            // --n;  // можно этого и не делать
        }

        mult *= mult;
        n /= 2;  // убиваем нулевой бит в двоичном представлении числа "n"
    }

    return res;
}

// TODO: implement stress test

int main() {
    uint64_t x = 5;
    uint64_t n = 19;
    // 19073486328125
    uint64_t res = fast_pow(x, n);
    cout << x << "^" << n << " = " << res << endl;

    cout << "MAX value for unsigned: " << numeric_limits<unsigned>::max() << endl;
    cout << "MIN value for unsigned: " << numeric_limits<unsigned>::min() << endl;

    // cout << "============================" << endl;

    // cout << "MAX value for uint: " << numeric_limits<uint>::max() << endl;
    // cout << "MIN value for uint: " << numeric_limits<uint>::min() << endl;

    // cout << "============================" << endl;

    // cout << "MAX value for long: " << numeric_limits<long>::max() << endl;
    // cout << "MIN value for long: " << numeric_limits<long>::min() << endl;

    return 0;
}
