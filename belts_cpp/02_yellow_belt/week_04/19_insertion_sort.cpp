#include <algorithm>  // std::iter_swap
#include <iostream>
#include <iterator>  // std::distance
#include <random>    // std::random_device
#include <sstream>
#include <vector>

using namespace std::literals;

#define _GLIBCXX_DEBUG 1  // включить режим отладки (запуск stress-теста)

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = ", "s) {
    bool isFirst = true;
    std::ostringstream oss;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << delimeter;
        }
        isFirst = false;
        oss << elm;
    }
    return oss.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "["s << Join(v) << "]"s;
}

std::vector<int> GenerateRandomVector(int NumberCount, int minimum = 1, int maximum = 50) {
    std::random_device rd;
    std::mt19937 gen(rd());  // these can be global and/or static, depending on how you use random elsewhere

    std::vector<int> values(NumberCount);
    std::uniform_int_distribution<> dis(minimum, maximum);
    std::generate(values.begin(), values.end(), [&]() { return dis(gen); });
    return values;
}

// требуется реализовать алгоритм "Сортировка вставками" для массива
template <typename RandomAccessIt>
void InsertionSort(RandomAccessIt it_begin, RandomAccessIt it_end) {
    if (std::distance(it_begin, it_end) < 2) {
        return;
    }

    auto it1 = std::next(it_begin);
    for (; it1 != it_end; ++it1) {
        auto it2 = it1;
        for (; it2 != it_begin && *std::prev(it2) > *it2; --it2) {
            std::iter_swap(std::prev(it2), it2);
        }
    }
}

int main() {
#ifdef _GLIBCXX_DEBUG
    while (true) {
        std::vector<int> v = GenerateRandomVector(10);
        std::vector<int> v1 = v;
        std::vector<int> v2 = v;
        std::sort(v1.begin(), v1.end());
        InsertionSort(v2.begin(), v2.end());

        if (v1 == v2) {
            std::cout << "OK, v = " << v << std::endl;
        } else {
            std::cout << "Sorting FAILURE:"s << std::endl
                      << "v: "s << v << std::endl
                      << "v sorted by std::sort: "s << v1 << std::endl
                      << "v sorted by InsertionSort: "s << v2 << std::endl;
            break;
        }
    }
#endif  // _GLIBCXX_DEBUG

    // // исходный вектор, после сортировки должен иметь вид: [3, 11, 17, 19, 19]
    // std::vector<int> v = {19, 17, 11, 3, 19};

    // исходный вектор, после сортировки должен иметь вид: [17, 19]
    std::vector<int> v = {19, 17};
    InsertionSort(v.begin(), v.end());
    std::cout << v << std::endl;
    return 0;
}
