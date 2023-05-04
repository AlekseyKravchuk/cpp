#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std::literals;

template <typename InputIterator>
void PrintRange(InputIterator it_begin, InputIterator it_end, const std::string& delim = " "s) {
    bool isFirst = true;
    for (auto it = it_begin; it != it_end; ++it) {
        if (!isFirst) {
            std::cout << delim;
        }
        isFirst = false;
        std::cout << *it;
    }
    std::cout << std::endl;
}

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = " "s) {
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
    return os << "["s << Join(v, ", "s) << "]"s;
}

int main() {
    // Пусть имеется УПОРЯДОЧЕННАЯ (важно!) последовательность:
    // 1 2 3 4 4 4 5 5 5 5 5 6 7
    // std::vector<int> v = {1, 2, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 7};
    // int x = 5;
    std::vector<int> v = {5, 9, 11, 12, 17, 21, 21, 21, 25, 26, 26, 26, 26, 27};
    int x = 1;

    // auto it_lbound = std::lower_bound(v.begin(), v.end(), x);
    // std::cout << it_lbound - v.begin() << std::endl;

    auto it_ubound = std::upper_bound(v.begin(), v.end(), x);
    std::cout << "Upper bound for x = "s << x << ": "s << *it_ubound << std::endl;
    // std::cout << it_ubound - v.begin() << std::endl;

    // PrintRange(it_lbound, it_ubound);

    // int y = 26;
    // auto range = std::equal_range(v.begin(), v.end(), y);
    // PrintRange(range.first, range.second);


    return 0;
}
