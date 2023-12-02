#include <iostream>
#include <set>
#include <utility> // std::pair, std::move

int main() {
    using Priority = int;
    using Id = int;

    std::set<std::pair<int, int>, std::greater<std::pair<int, int>>> s1;
    s1.insert({Priority{3}, Id{0}});
    s1.insert({Priority{2}, Id{1}});
    s1.insert({Priority{5}, Id{2}});
    s1.insert({Priority{3}, Id{3}});

    for (auto [priority, id] : s1) {
        std::cout << priority << ": " << id << std::endl;
    }

    std::cout << "After changing priority for element with id = 0" << std::endl;
    auto node_handle = s1.extract({3, 0});
    node_handle.value().first += 20;

    auto x = node_handle.value();
    s1.insert(std::move(node_handle));

    std::cout << "After changing priority for element with id = 0" << std::endl;
    std::cout << "x.first = " << x.first << ", x.second = " << x.second << std::endl;

    for (auto [priority, id] : s1) {
        std::cout << priority << ": " << id << std::endl;
    }

    return 0;
}
