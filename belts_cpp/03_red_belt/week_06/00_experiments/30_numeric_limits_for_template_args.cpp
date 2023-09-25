#include <cmath>    // std::abs
#include <cstddef>  // size_t
#include <iostream>
#include <limits>  // std::numeric_limits

template <typename T>
inline int sgn(T val) {
    return ((T{} < val) - (val < T{}));
}

template <typename T>
struct Range {
    T min{};
    T max{};
    uint64_t size;

    Range(T start_range = std::numeric_limits<T>::min(),
          T end_range = std::numeric_limits<T>::max())
        : min(start_range),
          max(end_range) {
        if (sgn(min) == -1) {
            uint64_t digits = static_cast<uint64_t>(std::numeric_limits<T>::digits);
            size = 2ul << digits;
        } else if (sgn(min) == 0) {
            size = static_cast<uint64_t>(max) + 1ul;
        } else {
            // for user-defined input range for resulting map
            size = static_cast<uint64_t>(max) - static_cast<uint64_t>(min) + 1ul;
        }
    }
};

int main() {
    auto range = Range<int16_t>{};
    std::cout << "range: [" << range.min << "; " << range.max << "]" << std::endl;
    std::cout << "range_size of [" << range.min << "; " << range.max << "] = " << range.size << " elements;" << std::endl;

    return 0;
}
