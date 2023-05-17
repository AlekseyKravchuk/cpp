#include <iostream>
#include <iterator>  // std::next
#include <vector>

using namespace std::literals;

// Представим пару итераторов в виде шаблонного класса
template <typename Iterator>
struct IteratorRange {
    Iterator first;
    Iterator last;

    Iterator begin() const { return first; }
    Iterator end() const { return last; }
};

template <typename T>
size_t RangeSize(IteratorRange<T> it_range) {
    return std::distance(it_range.first, it_range.last);
}

// // Создание копии вектора нам не подходит в силу низкой эффективности этого подхода
// template <typename T>
// std::vector<T> Head(std::vector<T>& v, size_t prefix_size) {
//     return {v.begin(), std::next(v.begin(), std::min(v.size(), prefix_size))};
// }

// Нужен какой-то другой способ, который бы позволил вернуть из функции Head некоторый диапазон
// => мы може возвращать пару итераторов и итерироваться по ним
template <typename T>
IteratorRange<typename std::vector<T>::iterator> Head(std::vector<T>& v, size_t prefix_size) {
    return {v.begin(), std::next(v.begin(), std::min(v.size(), prefix_size))};
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};

    auto it_range = Head(v, 3);
    for (auto& elm : it_range) {
        elm += 100;
    }
    std::cout << "iterator range size: "s << RangeSize(it_range) << std::endl;

    // Хотим итерироваться не по всему вектору, а только по его первым трем элементам
    // => напрямую воспользоватьтся удобным циклом range-based for не получится
    for (const auto& elm : Head(v, 4)) {
        std::cout << elm << ' ';
    }
    std::cout << std::endl;

    for (const auto& elm : v) {
        std::cout << elm << ' ';
    }
    std::cout << std::endl;

    return 0;
}
