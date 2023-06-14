#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
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