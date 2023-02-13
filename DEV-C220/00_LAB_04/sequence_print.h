#pragma once

#include <iterator>  // std::begin
#include <type_traits>

using namespace std::literals;

template <typename Collection>
void PrintCollection(const Collection& collection) {
    using type = std::remove_reference_t<decltype(*std::begin(collection))>;

    bool isFirst = true;
    if constexpr (std::is_pointer_v<type>) {
        for (const auto& elm : collection) {
            if (isFirst) {
                std::cout << *elm;
                isFirst = false;
            } else {
                std::cout << " "s << *elm;
            }
        }
    } else {
        for (const auto& elm : collection) {
            if (isFirst) {
                std::cout << elm;
                isFirst = false;
            } else {
                std::cout << " "s << elm;
            }
        }
    }

    std::cout << std::endl;
}

template <typename Collection>
void PrintAdapter(const Collection& collection) {
    using ItemType = Collection::value_type;

    if constexpr (std::is_same_v<std::stack<ItemType>, Collection>) {
        std::cout << "stack" << std::endl;
        // реализация вывода с помощью методов, доступных для адаптера std::stack<ItemType>
    } else if (std::is_same_v<std::queue<ItemType>, Collection>) {
        std::cout << "queue" << std::endl;
        // реализация вывода с помощью методов, доступных для адаптера std::queue<ItemType>
    } else if (std::is_same_v<std::priority_queue<ItemType>, Collection>) {
        std::cout << "priority_queue" << std::endl;
        // реализация вывода с помощью методов, доступных для адаптера std::priority_queue<ItemType>
    }
}