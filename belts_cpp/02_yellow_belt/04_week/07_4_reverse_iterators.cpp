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

int main() {
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "C++"s, "Ada"s, "Java"s, "C++"s};

    // тестируем функцию "PrintRange"
    PrintRange(langs.begin(), langs.end());

    // в обратном порядке:
    PrintRange(langs.rbegin(), langs.rend());

    return 0;
}
