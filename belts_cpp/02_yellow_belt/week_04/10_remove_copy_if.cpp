#include <algorithm>
#include <iostream>
#include <iterator>  // std::inserter
#include <map>
#include <set>
#include <sstream>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

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

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{"s << Join(s, ", "s) << "}"s;
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    return os << "{"s << Join(m, ", "s) << "}"s;
}

template <typename IteratorType>
void PrintRange(IteratorType it_begin, IteratorType it_end, const std::string& delim = ", "s) {
    bool isFirst = true;
    for (; it_begin != it_end; ++it_begin) {
        if (!isFirst) {
            std::cout << delim;
        }
        isFirst = false;
        std::cout << *it_begin;
    }
    std::cout << std::endl;
}

bool f(int val) {
    return val >= 3 && val <= 9;
}

int main() {
    std::set<int> nums = {2, 8, 1, 5, 19, 7, 3, 8, 6};
    std::vector<int> v;

    // требуется скопировать в вектор "v" из множества "nums" все числа НЕ попадающие в диапазон [3, 9] в вектор "v"
    std::pair<int, int> range{3, 9};

    // // копирует диапазон элементов, ИСКЛЮЧАЯ те, которые СООТВЕТСТВУЮТ заданным критериям
    // std::remove_copy_if(nums.begin(), nums.end(),
    //                     std::back_inserter(v),
    //                     [range](int val) {
    //                         return val >= range.first && val <= range.second;
    //                     });
    // std::cout << v << std::endl;

    v.assign(nums.begin(), nums.end());
    std::sort(v.begin(), v.end(),
              [](int lhs, int rhs) {
                  return f(lhs) > f(rhs);
              });

    auto it = std::partition_point(begin(v), end(v), f);
    v.erase(begin(v), it);

    std::cout << v << std::endl;

    return 0;
}
