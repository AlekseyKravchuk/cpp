#include <iostream>
#include <map>
#include <sstream>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

// требуется написать шаблонную функцию позволяющую выводить на экран содержимое
// основных контейнеров: std::vector<T>, std::map<KeyType, ValueType>

// Требования, предъявляемые к коллекции:
// 1. По ней можно итерироваться с помощью цикла "range-based for".
// 2. Её (коллекции) элементы можно выводить в поток.
template <typename Collection>
std::string Join(const Collection& collection, const std::string& separator = ", "s) {
    std::ostringstream oss;
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            oss << elm;
            isFirst = false;
        } else {
            oss << separator << elm;
        }
    }
    return oss.str();
}

// перегружаем "operator<<" для "std::vector<T>"
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T> v) {
    return os << "["s << Join(v) << "]"s;
}

// перегружаем "operator<<" для "std::map<KeyType, ValueType>"
template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType> m) {
    return os << "{"s << Join(m) << "}"s;
}

// перегружаем "operator<<" для "std::pair<FirstType, SecondType>"
template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<const FirstType, SecondType>& p) {
    os << "("s << p.first << ", "s << p.second << ")"s;
    return os;
}

int main() {
    // std::vector<int> v{3, 1, 9, 12, 4, 8, 5};
    std::vector<double> v{3, 1, 9.1, 12, 4.5, 8, 5};
    std::cout << v << std::endl;

    // хотим выводить словари
    // std::map<std::string, int> m{
    //     {"one"s, 1},
    //     {"two"s, 2},
    //     {"three"s, 3},
    //     {"four"s, 4}};
    std::map<std::string, double> m{
        {"one"s, 1.2},
        {"two"s, 2.4},
        {"three"s, 3.1},
        {"four"s, 4.3}};
    std::cout << m << std::endl;

    // пробуем вывести вектор векторов
    std::vector<std::vector<int>> vv {
        {1, 2, 3, 4},
        {5, 6, 7, 8}, 
        {9, 10, 11, 12}
    };
    std::cout << vv << std::endl;

    return 0;
}
