#pragma once

#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std::literals;

// ==================================================================================================
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
// ==================================================================================================

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
// ==================================================================================================

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "["s << Join(v, ", "s) << "]"s;
    return os;
}
// ==================================================================================================

template <typename First, typename Second>
std::ostream& operator<<(std::ostream& os, const std::pair<First, Second> p) {
    return os << "<"s << p.first << ", "s << p.second << ">"s;
}

// ==================================================================================================

template <class T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& lst) {
    return os << "list["s << Join(lst, ", "s) << "]"s;
}
// ==================================================================================================

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType> m) {
    return os << "{"s << Join(m) << "}"s;
}
// ==================================================================================================

template <typename KeyType>
std::ostream& operator<<(std::ostream& os, const std::set<KeyType>& s) {
    return os << "{" << Join(s, ", ") << "}";
}
// ==================================================================================================

template <typename KeyType>
std::ostream& operator<<(std::ostream& os, const std::multiset<KeyType>& m_set) {
    return os << "{" << Join(m_set, ", ") << "}";
}
// ==================================================================================================

template <typename KeyType, typename HasherType>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<KeyType, HasherType>& u_set) {
    return os << "{" << Join(u_set, ", ") << "}";
}
// ==================================================================================================

template <typename KeyType, typename ValueType, typename HasherType>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<KeyType, ValueType, HasherType>& u_map) {
    return os << "{" << Join(u_map, ", ") << "}";
}
// ==================================================================================================

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<const FirstType, SecondType>& p) {
    os << "("s << p.first << ", "s << p.second << ")"s;
    return os;
}
// ==================================================================================================