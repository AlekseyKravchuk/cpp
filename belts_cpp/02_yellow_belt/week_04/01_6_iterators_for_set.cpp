#include <algorithm>
#include <iostream>
#include <sstream>
#include <set>

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
    // Пусть имеется некоторый элемент в векторе, который мы хотим найти
    // Требуется вывести все элементы, которые идут до этого найденного элемента, и все элементы после этого элемента, включая его самого
    std::set<std::string> langs =
        {"Python"s, "C++"s, "C"s, "Java"s, "C#", "Go"s, "Haskel"s};

    auto it_found = std::find(langs.begin(), langs.end(), "Java"s);

    // в "std::set" все элеметы хранятся в отсортированном виде
    // выводим все строки, которые лексикографически меньше, чем "Java"
    PrintRange(langs.begin(), langs.end());

    // элементы ДО найденного (исключая его)
    // выводим все строки, которые лексикографически больше, либо равны "Java"
    PrintRange(langs.begin(), it_found, ", "s);

    // элементы ПОСЛЕ найденного (ВКЛЮЧАЯ найденный элемент)
    PrintRange(it_found, langs.end(), ", "s);

    return 0;
}
