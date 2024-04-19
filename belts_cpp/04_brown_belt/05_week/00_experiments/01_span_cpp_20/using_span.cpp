#include <stdlib.h>  // bsearch

#include <cstddef>
#include <iostream>
#include <span>
#include <vector>

using namespace std;

int SumIntArray_C_style(int* arr_ptr, size_t num_contiguous_elements) {
    int result{0};
    for (size_t i = 0; i < num_contiguous_elements; ++i) {
        result += arr_ptr[i];
    }

    return result;
}

int SumIntArray_CPP_span(std::span<int> items) {
    int result{0};
    for (const auto& item : items) {
        result += item;
    }

    return result;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int sum = SumIntArray_C_style(arr, (sizeof(arr) / sizeof(arr[0])));
    cout << "sum = " << sum << endl;

    int sum_for_span = SumIntArray_CPP_span(arr);
    cout << "sum_for_span = " << sum_for_span << endl;

    return 0;
}
