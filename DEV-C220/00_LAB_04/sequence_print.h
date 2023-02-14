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
    using ItemType = typename Collection::value_type;
    bool isFirst = true;

    if constexpr (std::is_same_v<std::stack<ItemType>, Collection>) {
        std::cout << "stack:" << std::endl;

        auto my_stack = collection;
        static_assert(std::is_same_v<decltype(my_stack), Collection>);

        while (!my_stack.empty()) {
            if (isFirst) {
                std::cout << my_stack.top();
                my_stack.pop();
                isFirst = false;
            } else {
                std::cout << ' ' << my_stack.top();
                my_stack.pop();
            }
        }
        std::cout << std::endl;

    } else if constexpr (std::is_same_v<std::queue<ItemType>, Collection>) {
        std::cout << "queue:" << std::endl;

        auto my_queue = collection;
        static_assert(std::is_same_v<decltype(my_queue), Collection>);
        while (!my_queue.empty()) {
            if (isFirst) {
                std::cout << my_queue.front();
                my_queue.pop();
                isFirst = false;
            } else {
                std::cout << ' ' << my_queue.front();
                my_queue.pop();
            }
        }
        std::cout << std::endl;

    } else if constexpr (std::is_same_v<std::priority_queue<ItemType>, Collection>) {
        std::cout << "priority_queue:" << std::endl;
        auto my_pr_queue = collection;
        static_assert(std::is_same_v<decltype(my_pr_queue), Collection>);

        while (!my_pr_queue.empty()) {
            if (isFirst) {
                std::cout << my_pr_queue.top();
                my_pr_queue.pop();
                isFirst = false;
            } else {
                std::cout << ' ' << my_pr_queue.top();
                my_pr_queue.pop();
            }
        }

        std::cout << std::endl;
    }
}