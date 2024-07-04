#include <iostream>
#include <cstdint>
#include <limits>
#include <optional>

using namespace std;

template <typename T>
optional<T> Add(T lhs, T rhs) {
    if (lhs > 0 && rhs > 0) {
        if (lhs > numeric_limits<T>::max() - rhs) {
            return nullopt;
        }
    } else if (lhs < 0 && rhs < 0) {
        if (lhs < numeric_limits<T>::min() - rhs) {
            return nullopt;
        }
    }

    return lhs + rhs;
}

int main() {
    int64_t lhs;
    int64_t rhs;

    cin >> lhs >> rhs;

    if (auto sum = Add(lhs, rhs)) {
        cout << sum.value() << endl;
    } else {
        cout << "Overflow!" << endl;
    }
}

