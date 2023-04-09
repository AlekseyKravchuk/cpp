#include <algorithm>
#include <iostream>
#include <sstream>
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

int main() {
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "C++"s, "Ada"s, "Java"s, "C++"s};

    // хотим найти МИНИМАЛЬНЫЙ элемент в векторе
    auto it_min = std::min_element(langs.begin(), langs.end());
    std::cout << "Min element in vector "s << langs << ": "s << *it_min << std::endl;

    // хотим найти МАКСИМАЛЬНЫЙ элемент в векторе
    auto it_max = std::max_element(langs.begin(), langs.end());
    std::cout << "Max element in vector "s << langs << ": "s << *it_max << std::endl;

    // хотим найти одновременно и МИНИМАЛЬНЫЙ, и МАКСИМАЛЬНЫЙ элементы:
    auto p = std::minmax_element(langs.begin(), langs.end());
    std::cout << "Min: "s << *p.first << ", max: " << *p.second << std::endl;

    return 0;
}
