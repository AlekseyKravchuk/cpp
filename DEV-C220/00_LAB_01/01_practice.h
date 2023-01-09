#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <map>
#include <set>

using namespace std::literals;

// std::ostream& operator<<(std::ostream& os, std::pair<char, std::set<std::string>> p) {
//     os << "{"s << p.first << ": " << 
// }

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

template <typename Collection>
void NegateAll(Collection& collection) {
    for (auto& elm : collection) {
        elm = -elm;
    }
}
