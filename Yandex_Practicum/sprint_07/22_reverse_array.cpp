#include <algorithm>
#include <cassert>
#include <vector>

// Напишите шаблонную функцию ReverseArray, которая заменяет порядок элементов массива на противоположный.
// Функция принимает адрес первого элемента массива и количество элементов массива.
template <typename T>
void ReverseArray(T* start, size_t size) {
    using namespace std;

    for (size_t i = 0; i < size / 2; ++i) {
        std::swap(start[i], start[size - 1 - i]);
    }
}

int main() {
    using namespace std;

    // vector<int> v = {1, 2};
    vector<int> v = {1, 2, 3, 4};
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{4, 3, 2, 1}));
}