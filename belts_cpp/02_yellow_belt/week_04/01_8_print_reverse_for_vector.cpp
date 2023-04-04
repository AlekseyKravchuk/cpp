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

int main() {
    // Хотим найти в вектора "langs" какой-то ЯП, который начинается с буквы 'C'
    // или сказать, что такого языка в векторе нет
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "Java"s, "C#"};

    std::cout << Join(langs) << std::endl;
    // PrintRange(langs.rbegin(), langs.rend());

    auto it = langs.end();
    while (it != langs.begin()) {
        --it;
        std::cout << *it << " ";
    }

    return 0;
}
