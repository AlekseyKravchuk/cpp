#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <utility>


using namespace std::literals;

// в случае использования std::map нужно перегрузить stream insertion operator, "<<", для std::pair
template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::pair<const Key, Value>& p) {
    os << "{"s << p.first << ", "s << p.second << "}"s
       << " "s;

    return os;
}

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt, const std::string& message = ""s) {
    if (!message.empty()) {
        std::cout << message;
    }

    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

template <typename Collection>
void PrintCollection(const Collection& collection, const std::string& message = ""s) {
    if (!message.empty()) {
        std::cout << message;
    }

    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << " "s << elm;
        }
    }
    std::cout << std::endl;
}

// TODO: FIX WRONG IMPLEMENTATION
// template <typename Collection>
// void NegateAll(Collection& collection) {
//     auto it_begin = std::begin(collection);

//     if constexpr (std::is_same_v(it_begin::value_type, std::string)) {
//         for (auto& ch : collection) {
//             if (std::isalpha(ch)) {
//                 if (std::islower(ch)) {
//                     ch = std::toupper(ch);
//                 } else {
//                     ch = std::tolower(ch);
//                 }
//             }
//         }
//     } else {
//         for (auto& elm : collection) {
//             elm = -elm;
//         }
//     }
// }

template <typename Collection>
void NegateAll(Collection& collection) {
    for (auto& elm : collection) {
        elm = -elm;
    }
}
