#include <algorithm>
#include <cstddef>
#include <vector>

#include "../print.h"

template <typename T>
void ReversedCopy(T* src_begin, size_t count, T* dst) {
    T* src_last = src_begin + count;

    for (; src_begin != src_last; ++dst) {
        *dst = *(--src_last);
    }
}

int main() {
    //               0  1  2  3  4  5  6 
    std::vector v = {1, 2, 3, 4, 5, 6, 7};
    PrintRange(v.begin(), v.end());

    size_t count = 7;
    ReversedCopy(&*v.begin(), count - 1, &v[1]);
    PrintRange(v.begin(), v.end());

    return 0;
}
