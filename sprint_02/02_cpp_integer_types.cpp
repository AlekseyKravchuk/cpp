#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    // const auto min = numeric_limits<int64_t>::min();   // минимальное значение типа int64_t
    // const auto max = numeric_limits<uint64_t>::max();  // максимальное значение типа uint64_t
    // cout << min << endl;
    // cout << max << endl;
    // cout << min + max << endl;
    // cout << 2*min << endl;
    // cout << 2*max << endl;

    // const auto min1 = numeric_limits<int>::min();   // минимальное значение типа int64_t
    // const auto min2 = numeric_limits<uint>::min();  // максимальное значение типа uint64_t
    // cout << min1 << endl;
    // cout << min2 << endl;

    cout << "sizeof(uint8_t) = "s << sizeof(uint8_t) << " bytes, range: ["
         << static_cast<int>(numeric_limits<uint8_t>::min()) << ", "s
         << static_cast<int>(numeric_limits<uint8_t>::max()) << "] "s << endl;
}