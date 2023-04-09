#include <algorithm>
#include <iostream>
#include <iterator>  // std::inserter
#include <map>
#include <set>
#include <sstream>
#include <utility>  // std::pair
#include <vector>
#include <cctype>  // std::isupper

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

int main() {
    // std::vector<std::string> langs =
    //     {"Python"s, "C++"s, "C"s, "C++"s, "Ada"s, "Java"s, "C++"s};

    std::set<std::string> langs =
        {"Python"s, "C++"s, "C"s, "C++"s, "ada"s, "Java"s, "C++"s};

    // хотим удалить все языки, начинающиеся на букву 'C'
    // алгоритм "std::remove_if" фактически ничего не удаляет!!!, а только перемещает элементы, подлежащие удалению в конец контейнера
    // общие алгортмы, которые принимают итераторы, также ничего не удаляют - они не могут изменять размер контейнера


    // НО! Для std::set нельзя вызвать алгоритм "std::remove" или "std::remove_if", поскольку эти алгоритмы ПЕРЕСТАВЛЯЮТ элементы в исходном контейнере
    // Множество гарантирует отсортированность своих элементов
    // Т.е. множество нельзя передавать в те алгоритмы, которые каким-либо образом изменяют переданный в них диапазон
    // langs.erase(std::remove_if(langs.begin(), langs.end(),
    //             [](const std::string& s) { return s[0] == 'C'; }),
    //             langs.end());

    // std::cout << langs << std::endl;

    // для множества можно вызывать алгоритмы, которые позволяют проверить соблюдение какого-либо свойства для переданного диапазона
    // хотим проверить, что все ЯП начинаются с заглавной буквы
    auto isCriterionSatisfied = std::all_of(langs.begin(), langs.end(),
                          [](const std::string& s){
                            return std::isupper(s[0]);
                          });
    std::cout << std::boolalpha << isCriterionSatisfied << std::endl;

    return 0;
}
