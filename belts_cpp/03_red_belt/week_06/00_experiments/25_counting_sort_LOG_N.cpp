#include <algorithm>
#include <iostream>
#include <vector>


// tag: fast_sort
/*
Сортировка ОБЫЧНО работает за O(Nlog(N)), и это достаточно медленно.
Для натуральных чисел, когда разница между минимальными и максимальным числамиз в диапазоне небольшая,
можно огранизовать более эффектвные способы сортировки.
O(N)
*/
template <typename TIterator>
void CountingSort(TIterator first, TIterator second) {
    if (first == second) {
        return;
    }

    auto [min_it, max_it] = std::minmax_element(first, second);
    auto min_val = *min_it;
    auto max_val = *max_it;

    std::vector<unsigned int> count(max_val - min_val + 1, 0);
    for (auto it = first; it != second; ++it) {
        ++count[*it - min_val];
    }

    for (auto i = min_val; i <= max_val; ++i) {
        for (auto j = 0U; j < count[i - min_val]; ++j) {
            *(first++) = i;
        }
    }
}

int main() {
    // std::vector<int> data = {9, 7, 1, 5, 4, 8, 3, 2, 6};
    std::vector<int> data = {9, 7, 1, 5, 4, 8, 3, 2, 1, 4, 9, 2, 6};
    CountingSort(data.begin(), data.end());

    for (auto elem : data) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
