#include <deque>
#include <iostream>
#include <iterator>  // std::next
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

// благодаря добавлению конструктора с параметрами избавились от необходимости написания порождающей функции "MakeRange"
// в конструкторе должны использоваться ВСЕ шаблонные параметры
template <typename Iterator>
class IteratorRange {
   public:
    // конструктор, принимающий парамтеры ВСЕХ шаблонных типов
    IteratorRange(Iterator first, Iterator last)
        : _it_first(first), _it_last(last) {}

    Iterator begin() const { return _it_first; }
    Iterator end() const { return _it_last; }

   private:
    Iterator _it_first;
    Iterator _it_last;
};

template <typename T>
size_t RangeSize(IteratorRange<T> it_range) {
    return std::distance(it_range.begin(), it_range.end());
}

// в таком виде шаблонная функция "Head" позволяет итерироваться ТОЛЬКО по префиксу ВЕКТОРА,
// хотим расширить функционал функции "Head"
// template <typename T>
// IteratorRange<typename std::vector<T>::iterator> Head(std::vector<T>& v, size_t prefix_size) {
//     return {v.begin(), std::next(v.begin(), std::min(v.size(), prefix_size))};
// }

// // эта версия "Head" не позволяет работать с констатнтыми итераторами
// template <typename Container>
// IteratorRange<typename Container::iterator> Head(Container& c, size_t prefix_size) {
//     return {c.begin(), std::next(c.begin(), std::min(c.size(), prefix_size))};
// }

template <typename Container>
auto Head(Container& c, size_t prefix_size) {
    return IteratorRange{c.begin(), std::next(c.begin(), std::min(c.size(), prefix_size))};
}


// порождающая функция
template <typename Iterator>
IteratorRange<Iterator> MakeRange(Iterator it_begin, Iterator it_end) {
    return IteratorRange<Iterator>{it_begin, it_end};
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    std::set<std::string> words = {"one"s, "two"s, "three"s, "four"s};

    // делаем дек константным
    const std::deque<int> nums = {5, 7, 23, 8, 10, 5, 6, 1};

    IteratorRange second_half_vector(v.begin() + v.size() / 2, v.end());

    std::cout << "second half of vector: "s;
    for (int x : second_half_vector) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    // =================================================================

    for (const auto& word : Head(words, 2)) {
        std::cout << word << ' ';
    }
    std::cout << std::endl;

    // =================================================================
    for (const auto& num : Head(nums, 4)) {
        std::cout << num << ' ';
    }
    std::cout << std::endl;

    return 0;
}
