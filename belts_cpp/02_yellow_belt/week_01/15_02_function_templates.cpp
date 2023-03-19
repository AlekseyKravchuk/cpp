#include <iostream>
#include <map>
#include <sstream>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

template <typename Collection>
std::string Join(const Collection& collection, const std::string delimeter) {
    std::ostringstream oss;
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            oss << elm;
            isFirst = false;
        } else {
            oss << delimeter << elm;
        }
    }

    return oss.str();
}

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<const FirstType, SecondType>& p) {
    return os << "("s << p.first << ", "s << p.second << ")"s;
}

// вывод контейнера "std::vector"
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T> v) {
    return os << "["s << Join(v, ", "s) << "]"s;
}

// вывод контейнера "std::map"; у std::map 2 шаблонных аргумента
template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    return os << "{"s << Join(m, ", "s) << "}"s;
}

int main() {
    std::vector<double> v{2, 5.1, 4.1, 3, 18, 1};
    std::cout << v << std::endl;

    std::map<double, std::string> m{
        {1, "one"s},
        {2.6, "two"s},
        {3, "three"s}};
    std::cout << m << std::endl;

    return 0;
}
